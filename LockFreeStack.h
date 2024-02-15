#ifndef LOCK_FREE_STACK_H
#define LOCK_FREE_STACK_H
#include <atomic>
#include <memory>
using std::atomic;

namespace mpcs51044 {
// Linked list of integers
struct StackItem {
  StackItem(int val) : next(0), value(val) {}
  StackItem *next; // Next item, 0 if this is last
  int value;
};

struct StackHead {
  StackItem *link; // First item, 0 if list empty
  unsigned count;  // How many times the list has changed (see lecture notes)
};

struct Stack {
  Stack();
  int pop();
  void push(int);

private:
  atomic<StackHead> head;
};

Stack::Stack() {
  StackHead init;
  init.link = nullptr;
  init.count = 0;
  head.store(init);
}

// Pop value off list
int Stack::pop() {
  // What the head will be if nothing messed with it
  StackHead expected = head.load();
  StackHead newHead;
  bool succeeded = false;
  while (!succeeded) {
    if (expected.link == 0) {
      return 0; // List is empty
    }
    // What the head will be after the pop:
    newHead.link = expected.link->next;
    newHead.count = expected.count + 1;
    // Even if the compare_exchange fails, it updates expected.
    succeeded = head.compare_exchange_weak(expected, newHead);
  }
  int value = expected.link->value;
  delete expected.link;
  return value;
}

// Push an item onto the list with the given head
void Stack::push(int val) {
  StackItem *newItem = new StackItem(val);
  StackHead oldHead = head.load();
  StackHead newHead;
  bool succeeded = false;
  while (!succeeded) {
    newItem->next = oldHead.link; // Point new item's next to the current head
    newHead.link = newItem;       // New head should now point to the new item
    newHead.count = oldHead.count + 1; // Increment operation count
    // Try to set the head to the new head. If the head hasn't changed since we
    // loaded it, this succeeds. If it fails, compare_exchange_weak
    // automatically reloads oldHead with the current head, so we can try again
    // with updated values.
    succeeded = head.compare_exchange_weak(oldHead, newHead);
  }
}
}
#endif