#include <iostream>
#include <limits>
#include <type_traits>

//!!Compile time process should meet the if constexpr condition
// for example if we put numeric limits in the if condition, string cannot have numeric limits.

template <typename T>
auto print_type_info( const T& t ) {
  if constexpr ( std::is_integral_v<T> && !std::is_same_v<bool, T> && std::numeric_limits<T>::min() < 1000 ) {
    return t + 1;
  } else if constexpr ( std::is_floating_point_v<T> ) {
    return t + 0.1;
  } else {
    return t;
  }
}

template <typename T>
constexpr bool is_both() {
  if constexpr ( std::is_integral_v<T> && !std::is_same_v<bool, T> ) {
    if constexpr ( std::numeric_limits<T>::max() < 1000 ) {
      return true;
    }
  }
  return false;
}

// real world example

int main() {
  std::cout << print_type_info( 5 ) << '\n';
  std::cout << print_type_info( 2.3 ) << '\n';
  std::cout << print_type_info( false ) << '\n';
  std::cout << print_type_info( "true" ) << '\n';
}
