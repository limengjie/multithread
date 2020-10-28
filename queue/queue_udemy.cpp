#include <iostream>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

using namespace std;

template<typename T>
class thread_safe_queue {
   mutex m;
   condition_variable cv;
   queue<shared_ptr<T>> queue;

public:
   thread_safe_queue() {

   }

   thread_safe_queue(thread_safe_queue const& other_queue) {

   }

   void push(T& value) {
      lock_guard<mutex> lg(m);
      queue.push(make_shared<T>(value));
      cv.notify_one();
   }

   shared_ptr<T> pop() {
      lock_guard<mutex> lg(m);
      if (queue.empty()) 
         return shared_ptr<T>();
      else
      {
         shared_ptr<T> res(queue.front());
         queue.pop();
         return res;
      }
   }


   bool empty() {

   }

   shared_ptr<T> wait_pop() {
      unique_lock<mutex> lg(m);
      cv.wait(lg, [] { return !queue.empty(); });
      shared_ptr<T> res = queue.front();
      queue.pop();

      return res;
   }

   size_t size() {

   }

};

int main() {

   return 0;
}
