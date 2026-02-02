#include <vector>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <array>
#include <sstream>
#include <utility>
#include <algorithm>

using namespace std;

//unordered_map<KeyType, ValueType>

/*
std::map: Keeps keys sorted alphabetically (like a phonebook). It is slower because it has to rearrange things every time you add a new item.

std::unordered_map: Dumps keys into "buckets" based on their hash. It is extremely fast (O(1)) because it doesn't care about order.
*/

class InvertedIndex{
    private:
        static const int NUM_SHARDS = 16; 
        
        struct Shard_unoptimal{
            unordered_map<string, vector<int>> index; // term -> list of document IDs
            shared_mutex mutex; // For thread-safe access
        };

        struct Shard{
            vector<pair<string, vector<int>>> index; // term -> list of document IDs
            bool is_sorted = false;
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

            // while(ss >> word){
            //     size_t shardIndex = getShardIndex(word);
            //     Shard& shard = shards[shardIndex];

            //     {
            //         unique_lock<shared_mutex> lock(shard.mutex);
            //         shard.index[word].push_back(docId);
            //     }
            // }

            while (ss>>word)
            {
                size_t shardIndex = getShardIndex(word);
                Shard& shard = shards[shardIndex];

                unique_lock<shared_mutex> lock(shard.mutex);

                shard.index.push_back({word, {docId}});
                shard.is_sorted = false;
                
            }
            
        }

        void finalize(){
            for(auto& shard:shards){
                unique_lock<shared_mutex> lock(shard.mutex);
                if(!shard.is_sorted){
                    sort(shard.index.begin(), shard.index.end(), [](const auto& a, const auto& b){
                        return a.first < b.first;
                    });
                    shard.is_sorted = true;
                }

                if (shard.index.empty()) continue;

                vector<pair<string, vector<int>>> compactIndex;
                compactIndex.reserve(shard.index.size());

                compactIndex.push_back(shard.index[0]);
                for(size_t i=1; i<shard.index.size(); ++i){
                    if(shard.index[i].first == compactIndex.back().first){
                        compactIndex.back().second.push_back(shard.index[i].second[0]);
                    }else{
                        compactIndex.push_back(shard.index[i]);
                    }
                }

                shard.index = move(compactIndex);
                shard.is_sorted = true;
            }
        }

        vector<int> lookup(const string& term){
            size_t shardIndex = getShardIndex(term);
            Shard& shard = shards[shardIndex];

            shared_lock<shared_mutex> lock(shard.mutex);

            if(!shard.is_sorted){
                return {};
            }

            auto it = lower_bound(shard.index.begin(), shard.index.end(), term, [](const auto& a, const string& b){
                return a.first < b;
            });

            if(it != shard.index.end() && it->first == term){
                return it->second;
            }
            return {};
        }

        vector<pair<int,int>> search(const string& term){
            vector<int> raw_results = lookup(term);

            unordered_map<int, int> scores;

            for(int docId : raw_results) {
                scores[docId]++;
            }   

            vector<pair<int, int>> sorted_results;
            for(const auto& entry : scores) {
                sorted_results.push_back({entry.first, entry.second});
            }

            sort(sorted_results.begin(), sorted_results.end(), [](const auto& a, const auto& b) {
                return b.second < a.second;
            });

            return sorted_results;
        }
};