#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mu;
condition_variable cond;
deque<int> buffer;
const unsigned int maxBufferSize = 50;

void producer(int val) {
   while (val) {
      unique_lock<mutex> locker(mu);
      cond.wait(locker, [](){return buffer.size() < maxBufferSize;});
      buffer.push_back(val);
      cout << "produced: " << val << endl;
      val--;
      locker.unlock();
      cond.notify_one();
   }
}

void consumer(int value) {
   while (value) {
      unique_lock<mutex> locker(mu);
      cond.wait(locker, [](){return buffer.size() > 0;});
      int val = buffer.back();
      buffer.pop_back();
      cout << "consumed: " << val << endl;
      value--;
      locker.unlock();
      cond.notify_one();
   }
}

int main() {
   thread t1(producer, 100);
   thread t2(consumer, 100);

   t1.join();
   t2.join();

   return 0;
}
