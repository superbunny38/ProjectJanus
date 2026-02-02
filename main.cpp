#include <iostream>
#include <thread>
#include <string>
#include "ModernAdapter.h"
#include "ThreadSafeQueue.h"
#include "InvertedIndex.h"

using namespace std;

int main() {
    //The Setup()
    ThreadSafeQueue<shared_ptr<ModernDoc>> documentQueue;//
    ModernAdapter adapter(documentQueue);
    InvertedIndex index;

    cout << "---Search Indexer Started---" << endl;

    /*
    thread: This launches a new timeline. Everything inside the { ... } runs in parallel with the rest of your main function.

    [&]: This is a "Capture List." It gives the thread permission to access variables from main (like adapter and documentQueue) by reference.

    Why shutdown()? This is crucial. When adapter.Run() finishes (after 20 items), the thread needs to yell "STOP!" If it didn't, the consumer would sit at the empty conveyor belt waiting forever.
    */
    {
        thread producerThread([&]() {
            adapter.Run();//Do the work
            documentQueue.shutdown(); // Signal that production is done
        });

        
        
        jthread consumerThread([&](){
            shared_ptr<ModernDoc> doc;
            while (documentQueue.pop(doc)) {
                index.addDocument(doc->id, doc->title + " " + doc->body);
            }
        });

        producerThread.join();
    }

    cout <<"---Performing Search for 'cloud'---" << endl;
    auto results = index.search("cloud");

    for (const auto& [docId, frequency] : results) {
        cout << "Document ID: " << docId << ", Frequency: " << frequency << endl;
    }

    cout << "---Search Indexer Stopped---" << endl;

    
    return 0;
}