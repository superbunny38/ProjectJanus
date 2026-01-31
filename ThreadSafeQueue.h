//Buffer
#include <queue>
#include <mutex>
#include <thread> 
#include <chrono>
#include <condition_variable>

using namespace std;

template <typename T>
class ThreadSafeQueue {
    private:
        queue<T> Q;
        mutable mutex Mutex;
        condition_variable Cv;
    
    public:
        void push(T value){
            {lock_guard<mutex> lock(Mutex);
            Q.push(move(value));}
            Cv.notify_one();
        }

        bool pop(T& result){
            unique_lock<mutex> lock(Mutex);
            cv.wait(lock, [this]{ return !Q.empty(); });
            
            result = move(Q.front());
            Q.pop();
            return true;
        }
};