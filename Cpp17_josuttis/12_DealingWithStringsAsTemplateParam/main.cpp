#include <iostream>
#include <complex>
#include <vector>
#include <string>

/*

Over time the different versions of C++ relaxed the rules for what can be used as templates parameters,
and with C++17 this happened again. Templates now can be used without the need to have
them defined outside the current scope.
*/

/*
For pointers, linkage is required, which means that you can’t pass string literals directly. However,
since C++17, you can have pointers with internal linkage. For example:
*/

template <const char* str>
class Message {};

extern const char hello[]= "Hello World!";  // external linkage
const char hello11[]= "Hello World!";       // internal linkage

void foo() {
  Message<hello> msg;      // OK (all C++ versions)
  Message<hello11> msg11;  // OK since C++11

  static const char hello17[]= "Hello World!";  // no linkage
  Message<hello17> msg17;                       // OK since C++17
}

/*
That is, since C++17, you still need two lines to pass a string literal to a template. But you can have
the first line in the same scope as the class instantiation.
This ability also solves an unfortunate constraint: While you could pass a pointer to a class template
since C++11:
*/

template <int* p>
struct A {};
int num;
A<&num> a;  // OK since C++11
// You couldn’t use a compile - time function that returned the address, which now is supported :

int num1;
constexpr int* pNum() { return &num1; }
A<pNum()> b;  // ERROR before C++17, now OK

void UsingStringsinTemplates() {}

int main() { return 0; }
