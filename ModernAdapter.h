#pragma once
#include "LegacyDataFeed.h"
#include "ThreadSafeQueue.h"
#include <string>
#include <vector>   
#include <memory>

using namespace std;

struct ModernDoc
{
    string title;
    string body;
    int id;
};

class ModernAdapter
{
    private:
    // a place to store the data we convert
    // We need to tell the class that documents is just a pointer/reference to the queue sitting in main.
        ThreadSafeQueue<shared_ptr<ModernDoc>>& documents;

    public:
        //constructor to accept ThreadSafeQueue as a reference
        ModernAdapter(ThreadSafeQueue<shared_ptr<ModernDoc>>& queue):documents(queue) {}

        static void OnDataReceived(LegacyDoc* doc, void* user_data){
            ModernAdapter* adapter = static_cast<ModernAdapter*>(user_data);
            shared_ptr<ModernDoc> modernDoc = make_shared<ModernDoc>();
            modernDoc->id = doc->id;
            modernDoc->title = string(doc->title);
            modernDoc->body = string(doc->body);
            adapter->documents.push(modernDoc);

            // Clean up legacy document memory
            delete[] doc->title;
            delete[] doc->body;
            delete doc;
        };

        void Run(){
            LegacyDataFeed legacyFeed;
            legacyFeed.StartStream(OnDataReceived, this);
        };
};
