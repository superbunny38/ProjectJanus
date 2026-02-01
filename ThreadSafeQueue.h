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
        bool shutdown_flag = false;
    
    public:
        void push(T value){
            {lock_guard<mutex> lock(Mutex);
            Q.push(move(value));}
            Cv.notify_one();
        }

        bool pop(T& result){
            unique_lock<mutex> lock(Mutex);
            Cv.wait(lock, [this]{ return !Q.empty() || shutdown_flag; });
            
            if (shutdown_flag && Q.empty()) {
                return false; // Indicate that the queue is shutting down
            }

            result = move(Q.front());
            Q.pop();
            return true;
        }
        void shutdown() {
            {lock_guard<mutex> lock(Mutex);
            shutdown_flag = true;}
            Cv.notify_all();
        }
};