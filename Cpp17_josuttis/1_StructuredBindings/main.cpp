#include <iostream>
#include <string>
#include <array>
#include "structbind1.h"

struct MyStruct{
    int i = 0;
    std::string s;
};


MyStruct getStruct()
{
    return MyStruct{42, "hello"};
}

int main()
{
    MyStruct ms;
    const auto& [u, v] = ms;

    auto [id, val] = getStruct(); // // id and val name i and s of returned struct
    if(id > 30)
    {
        std::cout << val << "\n";
    }


    //Move semantics is supported for structured bindings
    MyStruct ms1 = {42, "Jim"};
    auto [u1, v1] = std::move(ms1);

    std::cout << u1 << "\n";
    std::cout << ms1.i << "\n";

    /*
    In principle, structured bindings can be used for structures with public data members, raw C-style arrays, and “tuple-like objects:”
    */

    /*
    The examples introduced in the sections above demonstrate a couple of simple cases for structured
    bindings for structures and classes.
    Note that there is only limited usage of inheritance possible. All non-static data members must be
    members of the same class definition (thus, they have to be direct members of the type or of the same
    unambiguous public base class):
    */

    struct B{
        int a = 1;
        int b = 2;
    };

    struct D1 : B {
    };

    auto [x, y] = D1{};  // OK

    struct D2 : B{
        int c = 3;
    };

    // auto [i, j, k] = D2{}; // compile-time ERROR

    /*
    std::pair, std::tuple, and std::array
    The structured binding mechanism is extensible, so that you can add support for structured bindings
    to any type. The standard library uses this for std::pair<>, std::tuple<>, and std::array<>.
    */

    std::array<int, 4> stdarr {1,2,3,4};
    auto& [i, j, k, l] = stdarr;
    i += 10;
    std::cout << stdarr[0] << "\n";

    


    // std::map<std::string, int> coll;
    // ...
    // auto [pos, ok] = coll.insert({"new", 42})
    // if(!ok){}


    Customer cc("Tim", "Starr", 42);
    auto [ff, ll, vv] = cc;
    std::cout << "ff/ll/vv" << ff << ' ' << ll << ' ' << vv << '\n';

    // modify structured bindings:
    std::string ss= std::move(ff);
    ll = "Waters";
    vv += 10;
    std::cout << "ff/ll/vv" << ff << ' ' << ll << ' ' << vv << '\n';
    std::cout << "cc: " << cc.getFirst() << ' ' << cc.getLast() << ' ' << cc.getValue() << '\n';
    std::cout << "s: " << ss << '\n';

    return 0; 
}