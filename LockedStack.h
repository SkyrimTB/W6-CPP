#ifndef LOCKED_STACK_H
#  define LOCKED_STACK_H
#include<memory>
#include<mutex>
#include<stdexcept>
using std::scoped_lock;

namespace mpcs51044 {


struct LockedStack {
  struct ListItem {
    ListItem(std::unique_ptr<ListItem> &&n, int d) 
		: next(std::move(n)), data(d) {}
    std::unique_ptr<ListItem> next;
    int data;
  };
  
  LockedStack() {} // Don't really need this, since compiler would generate on its own

  int pop() {
    scoped_lock guard(lock);
    if(!first) {
      throw std::runtime_error("Can't pop from empty stack");
    }
    int result = first->data;
    first = std::move(first->next);
    return result;
  }

  void push(int i) {
    scoped_lock guard(lock);
    first = std::make_unique<ListItem>(std::move(first), i);
  }

private:
  std::unique_ptr<ListItem> first;
  std::mutex lock;
};
}
#endif