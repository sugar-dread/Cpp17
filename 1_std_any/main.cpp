#include <any>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

struct sss {
    sss( const sss& s ) = default;
    sss() = default;
};
// Important Note: The object has to have copy ctor to use any

int main() {
  std::vector<std::any> vec { 5, 3.4, std::string { "Hello World" }, sss() };

  std::cout << "sizeof any " << sizeof( std::any ) << "\n";

  std::cout << vec.size() << "\n";
  for ( const auto& elem : vec ) {
    std::cout << elem.type().name() << "\n";
  }

  static_assert( std::is_nothrow_move_constructible_v<sss> );

  int* i = std::any_cast<int>( &vec[0] );
  *i = 14;
  std::cout << "New value of any : " << std::any_cast<int>( vec[0] ) << "\n";

  std::any a( 12 );

  // set any value:
  a = std::string( "Hello!" );
  a = 16;
  // reading a value:

  // we can read it as int
  std::cout << std::any_cast<int>( a ) << '\n';

  // but not as string:
  try {
    std::cout << std::any_cast<std::string>( a ) << '\n';
  } catch ( const std::bad_any_cast& e ) {
    std::cout << e.what() << '\n';
  }

  // reset and check if it contains any value:
  a.reset();
  if ( !a.has_value() ) {
    std::cout << "a is empty!\n";
  }

  // you can use it in a container:
  std::map<std::string, std::any> mContainer;
  mContainer["integer"] = 10;
  mContainer["string"] = std::string( "Hello World" );
  mContainer["float"] = 1.0f;

  for ( auto& [key, val] : mContainer ) {
    if ( val.type() == typeid( int ) )
      std::cout << "int: " << std::any_cast<int>( val ) << "\n";
    else if ( val.type() == typeid( std::string ) )
      std::cout << "string: " << std::any_cast<std::string>( val ) << "\n";
    else if ( val.type() == typeid( float ) )
      std::cout << "float: " << std::any_cast<float>( val ) << "\n";
  }
}
