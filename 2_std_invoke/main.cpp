#include <functional>
#include <iostream>


int do_something( const int i ) { return 5 + i; }

struct obj {
    int j = 5;
    int do_something( const int i ) const { return j + i; }
    int do_something_2( const int i ) const { return j * i; }
};

int main() {
  std::cout << std::invoke( &do_something, 5 ) << "\n";
  obj ss;
  std::cout << ss.do_something( 3 ) << "\n";

  auto fp = &obj::do_something;

  std::cout << ( ss.*fp )( 30 ) << "\n";

  std::cout << std::invoke( &obj::do_something, ss, 10 ) << '\n';
  std::cout << std::invoke( &obj::do_something_2, ss, 20 ) << '\n';
}
