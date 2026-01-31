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
    thread processingThread(run);


    cout<<"Main thread is free to do other work while data is being processed..." << endl;

    processingThread.join();
    
    return 0;
}