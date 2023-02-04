# pragma once

#include "customer1.h"
#include <utility>

// provide a tuple-like API for class Customer for structured bindings:
template<>
struct std::tuple_size<Customer> {
    static constexpr int value = 3; //we have 3 attributes
};

template<>
struct std::tuple_element <2, Customer> {
    using type = long;  // last attribute is a long
};

template<std::size_t Idx>
struct std::tuple_element<Idx, Customer> {
    using type = std::string; // other attributes are strings
};


// define specific getters:
template<std::size_t> auto get(const Customer& c);
template<> auto get<0>(const Customer& c) {return c.getFirst();}
template<> auto get<1>(const Customer& c) {return c.getLast();}
template<> auto get<2>(const Customer& c) {return c.getValue();}