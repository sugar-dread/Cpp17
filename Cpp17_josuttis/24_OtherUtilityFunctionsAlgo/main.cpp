#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>  // for default_random_engine

template <typename T>
void generic_size_function(const T &coll) {
  // compute size:
  auto size { std::size(coll) };
  // advance to the position starting at the last 5 elements
  std::cout << size << " elems: ";
  auto pos { std::begin(coll) };
  if (size > 5) {
    std::advance(pos, size - 5);
    std::cout << "... ";
  }

  // print remaining elements:
  for (; pos != std::end(coll); ++pos) {
    std::cout << *pos << ' ';
  }
  std::cout << '\n';

  /*
      Finally, note that this code doesn’t work for forward_list<>, because forward lists don’t have a
      member function size(). So, if you only want to check whether the collection is empty, you better
      use std::empty(), which is discussed next.
  */
}

template <typename T>
void generic_data_function(const T &coll) {
  /*
  Finally, the new generic std::data() function allows us to give access to the raw data of containers
  that have a data() members and raw arrays. a container, a raw C array, or a std::initializer_list<>.
  For example, the following code print every second element:
  */

  // print every second element:
  for (std::size_t idx { 0 }; idx < std::size(coll); ++idx) {
    if (idx % 2 == 0) {
      std::cout << std::data(coll)[idx] << ' ';
    }
  }
  std::cout << '\n';
}

void clamp_func() {
  for (int i : { -7, 0, 8, 15 }) {
    std::cout << std::clamp(i, 5, 13) << '\n';
  }
}

void sample_func() {
  /*
  With sample() C++17 provides an algorithm that extracts a random subset (sample) from a given
  range of values (the population). This is sometimes called reservoir sampling or selection sampling.
  */

  // initialize a vector of 10,000 string values:
  std::vector<std::string> coll;
  for (int i= 0; i < 10000; ++i) {
    coll.push_back("value" + std::to_string(i));
  }

  // print 10 randomly selected values of this collection:
  std::sample(coll.begin(), coll.end(), std::ostream_iterator<std::string> { std::cout, "\n" }, 10, std::default_random_engine {});

  std::cout << "**************************\n";

  std::vector<std::string> coll1;
  for (int i= 0; i < 10000; ++i) {
    coll1.push_back("value" + std::to_string(i));
  }

  // initialize a Mersenne Twoster engine with a random seed:
  std::random_device rd;      // random seed (if supported)
  std::mt19937 eng { rd() };  // Mersenne twister engine

  // initialize destination range (must be big enough for 10 elements):
  std::vector<std::string> subset;
  subset.resize(100);
  // copy 10 randomly selected values from the source range to the destination range:
  // The return value end is initialized with contains the position behind the last randomly extracted
  // element, which can be used afterwards, for example, to print the extracted elements:
  auto end= std::sample(coll1.begin(), coll1.end(), subset.begin(), 100, eng);
  // print extracted elements (using return value as new end):
  for_each(subset.begin(), end, [](const auto &s) { std::cout << "random elem: " << s << '\n'; });
}

int main() {
  std::array arr { 27, 3, 5, 8, 7, 12, 22, 0, 55 };
  std::vector v { 0.0, 8.8, 15.15 };
  std::initializer_list<std::string> il { "just", "five", "small", "string", "literals" };
  generic_size_function(arr);
  generic_size_function(v);
  generic_size_function(il);
  generic_size_function("hello world");

  std::cout << "-------------\n";
  generic_data_function(arr);
  generic_data_function(v);
  generic_data_function(il);
  generic_data_function("hello world");

  std::cout << "-------------\n";
  clamp_func();
  std::cout << "-------------\n";
  sample_func();

  return 0;
}
