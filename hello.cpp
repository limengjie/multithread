#include <iostream>
#include <thread>

using namespace std;

void print()
{
   cout << "hello World from child thread\n";
}

int main() {
   thread t0(print);

   t0.join();

   return 0;
}
