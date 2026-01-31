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
        ThreadSafeQueue<shared_ptr<ModernDoc>> documents;

    public:
        //constructor
        ModernAdapter(){
            //accept threadsafequeue as a reference
            
        }
        //Add a public method to ModernAdapter (e.g., const vector<shared_ptr<ModernDoc>>& GetDocuments() const) so we can inspect the results.
        const vector<shared_ptr<ModernDoc>>& GetDocuments() const {
            return documents;
        }

        static void OnDataReceived(LegacyDoc* doc, void* user_data){
            ModernAdapter* adapter = static_cast<ModernAdapter*>(user_data);
            shared_ptr<ModernDoc> modernDoc = make_shared<ModernDoc>();
            modernDoc->id = doc->id;
            modernDoc->title = string(doc->title);
            modernDoc->body = string(doc->body);
            adapter->documents.push_back(modernDoc);

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
