#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <cstdio> 	//rand
#include <numeric> 	//accumulate

using namespace std;

atomic<long> multiThreadedSum(0);
const size_t input_size = 30003;
const size_t min_block_size = 1000;

void sumNumbers(const vector<int>& toBeSummed, int start, int end)
{
   for (int i = start; i < end; ++i)
      multiThreadedSum += toBeSummed[i];
}

int main()
{
   //init vector
   vector<int> toBeSummed;
   for (size_t i = 0; i < input_size; ++i)
      toBeSummed.push_back(rand() % 1000);

   //accumulate for comparison
   long sum = accumulate(toBeSummed.begin(), toBeSummed.end(), 0);
   cout << "The sum was " << sum << endl;
   
   //break down the vector into several blocks, each block is handled by a thread
   int num_threads_by_elements = input_size / min_block_size;
   int num_threads_by_hardware = thread::hardware_concurrency();
   int num_threads = min(num_threads_by_elements, num_threads_by_hardware);

   size_t block_size = input_size / num_threads;

   //process each block
   vector<thread> threads(num_threads);
   size_t start = 0, last;
   for (int i = 0; i < num_threads; ++i)
   {
      last = start + block_size;
      threads[i] = thread(sumNumbers, toBeSummed, start, last);
      start = last;
   }
   //process leftover
   sumNumbers(toBeSummed, start, toBeSummed.size());

   //call join function
   for (auto& thread : threads) thread.join();
   
   cout << "The sum is " << multiThreadedSum.load() << endl;

   return 0;
}
