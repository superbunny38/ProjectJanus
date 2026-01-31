#include "ModernAdapter.h"
#include <iostream>
#include <thread>

using namespace std;

void run(){
    ModernAdapter adapter;
    adapter.Run();

    const auto& docs = adapter.GetDocuments();
    for (const auto& doc : docs) {
        cout << "Document ID: " << doc->id << ", Title: " << doc->title << endl;
    }
}

int main() {
    thread t1(run);

    cout << "Main thread is doing other work..." << endl;

    t1.join();

    return 0;
}