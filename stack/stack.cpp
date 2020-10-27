#include <iostream>
#include <thread>
#include <memory>
#include <stdexcept>
#include <stack>
#include <mutex>

using namespace std;

template<typename T>
class thread_safe_stack {
   stack<shared_ptr<T>> stk;
   mutex m;

public:
   void push(T element)
   {
      lock_guard<mutex> lg(m);
      stk.push(make_shared<T>(element));
   }

   shared_ptr<T> pop()
   {
      lock_guard<mutex> lg(m);
      if (stk.empty())
      {
         throw runtime_error("stack is empty");
      }

      shared_ptr<T> res(stk.top());
      stk.pop();

      return res;
   }
   void pop(T& value)
   {
      lock_guard<mutex> lg(m);
      if (stk.empty())
      {
         throw runtime_error("stack is empty");
      }
      value = *(stk.top().get());
      stk.pop();
   }
};

int main() {

   thread_safe_stack<int> tss;
   tss.push(1);
   tss.push(2);
   tss.push(3);
   tss.pop();
   
   return 0;
}
