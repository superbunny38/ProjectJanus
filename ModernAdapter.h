#pragma once
#include "LegacyDataFeed.h"
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

    public:
    //constructor
        ModernAdapter() = default;

        void Run();
};
