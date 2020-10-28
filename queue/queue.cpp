#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

// A threadsafe-queue.

template <class T>
class SafeQueue
{
private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    c.wait(lock, [this]{return !q.empty();});
    T val = q.front();
    q.pop();
    return val;
  }
};

int main() {

   SafeQueue<int> sq;
   sq.enqueue(1);
   sq.enqueue(2);
   sq.enqueue(3);
   sq.dequeue();

   return 0;
}
