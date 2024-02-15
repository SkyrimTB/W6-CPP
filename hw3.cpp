#include <format>
#include <iostream>
#include <utility>

// Define the Counter class template
template <typename Func> class Counter {
  Func f;            // The function to be wrapped
  size_t _count = 0; // Counter for the number of calls

public:
  // Constructor
  explicit Counter(Func f) : f(f) {}

  // Overload the function call operator
  template <typename... Args>
  auto operator()(Args &&...args) -> decltype(f(std::forward<Args>(args)...)) {
    ++_count;
    return f(std::forward<Args>(args)...);
  }

  // Getter for the count
  size_t count() const { return _count; }
};

// CTAD guide to deduce the template argument from the constructor argument
template <typename Func> Counter(Func) -> Counter<Func>;

// Example function to be wrapped
int f(double d) { return static_cast<int>(2 * d); }

int main() {
  Counter cf(f);

  cf(1.3);
  cf(2.4);
  std::cout << std::format("f was called {} times\n", cf.count());
}
