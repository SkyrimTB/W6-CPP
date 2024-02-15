#include"DistributedCounter1.h"
#include<thread>
#include<iostream>
#include<chrono>
#include<vector>
#include<algorithm>
#include<iterator>
#include<utility>
using namespace mpcs;
using namespace std;
size_t const reps{100'000'000};
size_t const threadCount{10};
DistributedCounter c;
// How would I write jthread if I don't have it yet
// All it really needs is a joining destructor
struct MyJthread : public thread {
    template<typename Callable, typename ...Args>
    MyJthread(Callable&& c, Args&& ...args) 
      : thread(forward<Callable>(c), forward<Args>(args)...) {
    }
    ~MyJthread() { join(); }
};

struct MyJthread2 : public thread {
    using thread::thread; // Inherit constructors
    ~MyJthread2() { join(); }
};

void countALot()
{
  for (size_t s = 0; s < reps; s++)
    c++;
}

int main()
{
  auto start = chrono::high_resolution_clock::now();
  {
    vector<jthread> threads;
    for (size_t s = 0; s < threadCount; s++)
        threads.push_back(jthread(countALot));
  }
  auto end = chrono::high_resolution_clock::now();
  cout << "Count is " << c.get() << endl;
  cout << "Elapsed time is " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
  return 0;
}