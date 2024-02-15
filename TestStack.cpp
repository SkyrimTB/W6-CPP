#include "LockedStack.h"
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
using namespace mpcs51044;

LockedStack ls;

void useStack() {
  ls.push(1);
  ls.push(3);
  ls.push(5);
  for (int i = 0; i < 3; i++)
    cout << ls.pop() << endl;
}
int main() {
  jthread t1(useStack);
  jthread t2(useStack);
  return 0;
}