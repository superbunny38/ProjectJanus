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