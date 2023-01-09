#include <iostream>
#include <complex>
#include <vector>
#include <string>

/*
With the syntax if constexpr(. . . ), the compiler uses a compile-time expression to decide at compile
time whether to use the then part or the else part (if any) of an if statement. The other part (if
any) gets discarded, so that no code gets generated. This does not mean that the discarded part it is
completely ignored, though. It will be checked like code of unused templates.
*/

template <typename T>
std::string asString(T x) {
  if constexpr (std::is_same_v<T, std::string>) {
    return x;  // statement invalid, if no conversion to string
  } else if constexpr (std::is_arithmetic_v<T>) {
    return std::to_string(x);  // statement invalid, if x is not numeric
  } else {
    return std::string(x);  // statement invalid, if no conversion to string
  }
}

/*
Here, we use this feature to decide at compile time whether we just return a passed string, call
std::to_string() for a passed integral or floating-point value, or try to convert the passed argument
to std::string. Because the invalid calls are discarded, the following code compiles (which
would not be the case when using a regular run-time if):
*/

int main() {
  std::cout << asString(42) << '\n';
  std::cout << asString(std::string("hello")) << '\n';
  std::cout << asString("hello") << '\n';

  return 0;
}
