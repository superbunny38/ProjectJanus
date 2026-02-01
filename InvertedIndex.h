#include <vector>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <array>
#include <sstream>

using namespace std;

//unordered_map<KeyType, ValueType>


class InvertedIndex{
    private:
        static const int NUM_SHARDS = 16; 
        
        struct Shard{
            unordered_map<string, vector<int>> index; // term -> list of document IDs
            shared_mutex mutex; // For thread-safe access
        };

        array<Shard, NUM_SHARDS> shards;

        size_t getShardIndex(const string& word) const{
            return hash<string>{}(word) % NUM_SHARDS;
        }
    
    public:
        void addDocument(int docId, const string& content){   
            stringstream ss(content);
            string word;

            while(ss >> word){
                size_t shardIndex = getShardIndex(word);
                Shard& shard = shards[shardIndex];

                {
                    unique_lock<shared_mutex> lock(shard.mutex);
                    shard.index[word].push_back(docId);
                }
            }
        }
        vector<int> lookup(const string& term){
            size_t shardIndex = getShardIndex(term);
            Shard& shard = shards[shardIndex];

            shared_lock<shared_mutex> lock(shard.mutex);
            auto it = shard.index.find(term);
            if(it != shard.index.end()){
                return it->second;
            }else{
                return {};
            }
        }


};