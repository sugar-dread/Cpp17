#include <iostream>
#include <complex>
#include <vector>
#include <utility>

/*
Before C++17, you always have to explicitly specify all template parameter types for class templates.
For example, you can’t omit the double here:
std::complex<double> c{5.1,3.3};

omit the need to specify std::mutex here a second time:
std::mutex mx;
std::lock_guard<std::mutex> lg(mx);

Since C++17, the constraint that you always have to specify the template arguments explicitly was
relaxed. By using class template argument deduction (CTAD), you can skip defining the templates
arguments explicitly if the constructor is able to deduce all template parameters.

std::complex c{5.1,3.3}; // OK: std::complex<double> deduced

*/

void UsageofClassTemplateArgumentDeduction() {
  /*
  template<typename T>
  class complex {
  constexpr complex(const T& re = T(), const T& im = T());
  ...
  }
  */
  std::complex c1 { 1.1, 2.2 };  // deduces std::complex<double>
  std::complex c2 { 1.1, 2.2 };  // deduces std::complex<double>
  std::complex c3(2.2, 3.3);     // deduces std::complex<double>
  std::complex c4= 3.3;          // deduces std::complex<double>
  std::complex c5= { 4.4 };      // deduces std::complex<double>

  std::vector v1 { 1, 2, 3, 4, 5 };
  std::vector v2 { v1 };
  std::vector vv { v1, v2 };

  /* for variadic templates
  template<typename... Args>
    auto make_vector(const Args&... elems) {
    return std::vector{elems...};
    }
    std::vector<int> v{1, 2, 3};
    auto x1 = make_vector(v, v); // vector<vector<int>>
    auto x2 = make_vector(v); // vector<int> or vector<vector<int>> ?
    Currently, different compilers handle this differently and the issue is under discussion.

  */
}

template <typename CB>
class CountCalls {
 private:
  CB callback;    // callback to call
  long calls= 0;  // counter for calls

 public:
  explicit CountCalls(CB cb) : callback(cb) {}

  template <typename... Args>
  auto operator()(Args&&... args) {
    ++calls;
    return callback(std::forward<Args>(args)...);
  }

  long count() const { return calls; }
};

void DeducingtheTypeofLambdas() {
  // Here, the constructor, taking the callback to wrap, enables to deduce its type as template parameter CB.For example,
  // we can initialize an object passing a lambda as argument :
  // which means that the type of the sorting criterion sc is deduced as CountCalls<TypeOfTheLambda>.
  // This way, we can for example count the number of calls for a passed sorting criterion:
  CountCalls sc([](auto x, auto y) { return x > y; });
}

// Note that, unlike function templates, class template arguments may not be partially deduced (by
// explicitly specifying only some of the template arguments). For example:
template <typename T1, typename T2, typename T3= T2>
class C {
 public:
  C(T1 x= T1 {}, T2 y= T2 {}, T3 z= T3 {}) {}
};
void NoPartialClassTemplateArgumentDeduction() {
  // all deduced:
  C c1(22, 44.3, "hi");  // OK: T1 is int, T2 is double, T3 is const char*
  C c2(22, 44.3);        // OK: T1 is int, T2 and T3 are double
  C c3("hi", "guy");     // OK: T1, T2, and T3 are const char*

  // only some deduced:
  // C<string> c4("hi", "my");  // ERROR: only T1 explicitly defined
  // C<> c5(22, 44.3);          // ERROR: neither T1 not T2 explicitly defined
  // C<> c6(22, 44.3, 42);      // ERROR: neither T1 nor T2 explicitly defined

  C<std::string, std::string, int> c7;            // OK: T1,T2 are string, T3 is int
  C<int, std::string> c8(52, "my");               // OK: T1 is int,T2 and T3 are strings
  C<std::string, std::string> c9("a", "b", "c");  // OK: T1,T2,T3 are strings
}

int main() {
  UsageofClassTemplateArgumentDeduction();
  DeducingtheTypeofLambdas();

  return 0;
}
