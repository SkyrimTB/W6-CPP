#include <cstring>
#include <iostream>
#include <memory>


class A {
public:
  int value;
  A() : value(0) {}
  int func(int *p) { return *p; }
  int sum(double (*callback)(float &)) {
    float f = 5.0f;
    return static_cast<int>(callback(f));
  }
};

// Function to concatenate two strings
char *concatenate(char *s1, char *s2) {
  char *result = new char[std::strlen(s1) + std::strlen(s2) + 1];
  std::strcpy(result, s1);
  std::strcat(result, s2);
  return result;
}

// Example callback function for demonstration
double exampleCallback(float &f) { return f * 2.0; }

// Function to be used in function pointer array
void exampleFunc1() { std::cout << "Function 1 called" << std::endl; }

void exampleFunc2() { std::cout << "Function 2 called" << std::endl; }

int main() {
  // int*
  int i = 10;
  int *ip = &i;

  // int&
  int i2 = 20;
  int &ir = i2;

  // double
  double d = 3.14;

  // A*
  A *aPtr = new A;

  // char const*
  char const *cstr = "Hello";

  // char const&
  const char cc = 'A';
  const char &ccr = cc;

  // long[7]
  long larr[7] = {1, 2, 3, 4, 5, 6, 7};

  // int**
  int **ipp = &ip;

  // int*&
  int *&irp = ip;

  // float&
  float f = 5.5f;
  float &fr = f;

  // int(*)()
  int (*fp)() = []() -> int { return 42; };

  // int(*&)()
  int (*&rfp)() = fp;

  // char *(*)(char *, char *)
  char *(*concatFunc)(char *, char *) = concatenate;

  // int A::*
  int A::*mp = &A::value;

  // int (A::*)(int*)
  int (A::*mfp)(int *) = &A::func;

  // int (A::**)(int*)
  int (A::* * pmfp)(int *) = &mfp;

  // int (A::*&)(int*)
  int (A::* & rmfp)(int *) = mfp;

  // int (A::*)(double (*)(float &))
  int (A::*callbackMfp)(double (*)(float &)) = &A::sum;

  // void (*p[10]) (void (*)())
  void (*p[10])(void (*)()) = {nullptr};
  p[0] = [](void (*f)()) { f(); };

  // Demonstrating usage
  std::cout << "ip: " << *ip << std::endl;
  std::cout << "ir: " << ir << std::endl;
  std::cout << "d: " << d << std::endl;
  aPtr->*mp = 25; // Using pointer to member
  std::cout << "aPtr->*mp: " << aPtr->*mp << std::endl;
  delete aPtr; // Clean up dynamic memory

  char s1[] = "Hello, ";
  char s2[] = "World!";
  char *s3 = concatFunc(s1, s2); // Using function pointer
  std::cout << "Concatenated string: " << s3 << std::endl;
  delete[] s3; // Clean up dynamic memory

  int value = 10;
  std::cout << "A function result: " << (aPtr->*mfp)(&value) << std::endl;

  float callbackValue = 10.0f;
  A a;
  std::cout << "Callback function result: " << (a.*callbackMfp)(exampleCallback)
            << std::endl;

  // Calling a function from the array of function pointers
  p[0](exampleFunc1);
  p[0](exampleFunc2);

  return 0;
}
