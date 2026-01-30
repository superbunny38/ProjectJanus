#include "ModernAdapter.h"
#include <iostream>

using namespace std;

int main() {
    ModernAdapter adapter;
    adapter.Run();

    const auto& docs = adapter.GetDocuments();
    for (const auto& doc : docs) {
        cout << "Document ID: " << doc->id << ", Title: " << doc->title << endl;
    }

    return 0;
}