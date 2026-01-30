#pragma once
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

// Represents a document from the old system
struct LegacyDoc {
    char* title;
    char* body;
    int id;
};

// Function pointer type for the callback
typedef void (*DataCallback)(LegacyDoc* doc, void* user_data);

// Simulates a legacy system that pushes data to you via a callback
class LegacyDataFeed {
public:
    LegacyDataFeed() {}

    // Simulates fetching data from a slow disk/network
    // This function blocks!
    void StartStream(DataCallback callback, void* user_data) {
        const char* titles[] = {"C++ Pointers", "Rust vs C++", "Machine Learning Ops", "Docker Containers"};
        const char* bodies[] = {"Pointers are raw...", "Rust ensures safety...", "MLOps is critical...", "Containerization is key..."};
        
        for (int i = 0; i < 20; ++i) { // Simulates 20 records
            LegacyDoc* doc = new LegacyDoc(); // Allocates memory (You must clean this up!)
            doc->id = i;
            
            // Terrible legacy string copying
            doc->title = new char[50];
            doc->body = new char[50];
            sprintf(doc->title, "%s %d", titles[i % 4], i);
            sprintf(doc->body, "%s content %d", bodies[i % 4], i);

            // Invoke callback
            callback(doc, user_data);
            
            // Simulate IO latency (100ms)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};