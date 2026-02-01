#include <iostream>
#include <thread>
#include <string>
#include "ModernAdapter.h"
#include "ThreadSafeQueue.h"

using namespace std;

int main() {
    //The Setup()
    ThreadSafeQueue<shared_ptr<ModernDoc>> documentQueue;//
    ModernAdapter adapter(documentQueue);

    /*
    thread: This launches a new timeline. Everything inside the { ... } runs in parallel with the rest of your main function.

    [&]: This is a "Capture List." It gives the thread permission to access variables from main (like adapter and documentQueue) by reference.

    Why shutdown()? This is crucial. When adapter.Run() finishes (after 20 items), the thread needs to yell "STOP!" If it didn't, the consumer would sit at the empty conveyor belt waiting forever.
    */
    thread producerThread([&]() {
        adapter.Run();//Do the work
        documentQueue.shutdown(); // Signal that production is done
    });

    shared_ptr<ModernDoc> doc;
    cout << "---Search Indexer Started---" << endl;
    while (documentQueue.pop(doc)) {
        cout << "Indexed Document ID: " << doc->id << ", Title: " << doc->title << endl;
    }

    producerThread.join();
    cout << "---Search Indexer Stopped---" << endl;
    return 0;
}