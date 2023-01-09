#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <complex>
#include <set>
#include <utility>

/*
    std::any is a value type that is able to change its type, while still having type safety. That is,
    objects can hold values of any arbitrary type but they know which type the value has they currently
    hold. There is no need to specify the possible types when declaring an object of this type.

    As for std::optional<> and std::variant<> the resulting objects have value semantics. That
    is, copying happens deeply by creating an independent object with the current contained value and
    its type in its own memory. Because heap memory might be involved, copying a std::any usually
    is expensive and you should prefer to pass objects by reference or move values. Move semantics is
    partially supported.

*/


//Any Operations

void construction()
{
    std::any a1; // a1 is empty    
    std::any a2 = 42; // a2 contains value of type int
    std::any a3 = "hello"; // a2 contains value of type const char*

    // To hold a different type than the type of the initial value, you have to use the in_place_type tags:
    std::any a4{std::in_place_type<long>, 42};
    std::any a5{std::in_place_type<std::string>, "hello"};
    std::any a5b{std::in_place_type<const char[6]>, "hello"};

    std::any a6{std::complex{3.0, 4.0}};
    std::any a7{std::in_place_type<std::complex<double>>, 3.0, 4.0};


    // initialize a std::any with a set with lambda as sorting criterion:
    auto sc = [] (int x, int y) {
        return std::abs(x) < std::abs(y);
    };
    std::any a8{std::in_place_type<std::set<int,decltype(sc)>>, {4, 8, -7, -2, 0, 5}, sc};

    /*
    Note that there is also a convenience function make_any<>(), which can be used for single or multiple
    arguments (without the need for the in_place_type argument). You always have to explicitly
    specify the initialized type (it is not deduced if only one argument is passed):
    */
    auto a10 = std::make_any<float>(3.0);
    auto a11 = std::make_any<std::string>("hello");
    auto a13 = std::make_any<std::complex<double>>(3.0, 4.0);auto a14 = std::make_any<std::set<int,decltype(sc)>>({4, 8, -7, -2, 0, 5}, sc);
}

void accessValue()
{
    std::any a;

    a = 42; // a contains value of type int
    std::any b{std::in_place_type<std::string>, "hello"};

    std::any c = std::string{"hi"};

    std::cout << std::any_cast<std::string>(c) << '\n'; // yield copy of the value
    std::cout << std::any_cast<std::string&>(c) << '\n';  // write value by reference
    std::cout << std::any_cast<const std::string&>(c) << '\n'; // read-access by reference


}

int main()
{
    // Using std::any
    std::any a; // a is empty
    std::any b = 4.3; // b has value 4.3 of type double
    a = 4263; // a has value 42 of type int
    b = std::string{"hi"}; // b has value "hi" of type std::string

    if(a.type() == typeid(std::string))
    {
        std::string s = std::any_cast<std::string>(a);
        std::cout << s << '\n';
    }
    else if(a.type() == typeid(int))
    {
        try {
            std::cout << std::any_cast<int>(a) << '\n';
            auto t = std::any_cast<std::string>(a);
            std::cout << t << '\n';
        }
        catch (const std::bad_any_cast& e) {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
    }

    /*
    Note that std::any_cast<> creates an object of the passed type. If you pass std::string as
    template argument to std::any_cast<>, it creates a temporary string (a prvalue), which is then
    used to initialize the new object s. Without such an initialization, it is usually better to cast to a
    reference type to avoid creating a temporary object:
    */
    std::cout << std::any_cast<const std::string&>(b) << '\n'; 
    // To be able to modify the value, you need a cast to the corresponding reference type:
    std::any_cast<std::string&>(b) = "world";


    //You can also call std::any_cast for the address of a std::any object. In that case, the cast returns a corresponding pointer if the type fits or nullptr if not:
    
    if (auto p = std::any_cast<int>(&a); p) {
        std::cout << *p << '\n';
    }


    a.reset(); // makes it empty
    a = std::any{};
    a = {};
    if (a.has_value()) {}


    /*
    Note also that values are stored using their decayed type (arrays convert to pointers, and top-level
    references and const are ignored). For string literals this means that the value type is const char*.
    To check against type() and use std::any_cast<> you have to use exactly this type:
    */

    std::any vv = "hello"; // type() is const char*
    if (vv.type() == typeid(const char*)) { // true
    
    }
    if (vv.type() == typeid(std::string)) { // false

    }

    // std::cout << std::any_cast<const char*>(vv[1]) << '\n'; // OK
    // std::cout << std::any_cast<std::string>(vv[1]) << '\n'; // EXCEPTION


    /*
    These are more or less all operations. No comparison operators are defined (so, you can’t compare
    or sort objects), no hash function is defined, and no value() member functions are defined. And
    because the type is only known at run time, no generic lambdas can be used to deal with the current
    value independent from its type. You always need the run-time function std::any_cast<> to be
    able to deal with the current value, which means that you need some type specific code to reenter the
    C++ type system when dealing with values.
    */

    std::vector<std::any> vec;
    class test{
    public:
        int a = 123;
    };
    test x;
    vec.push_back(42);
    std::string str = "hello";
    vec.push_back(str);
    vec.push_back(x);


    for(const auto& e : vec)
    {
        if(e.type() == typeid(std::string)){
            std::cout << "string : " << std::any_cast<const std::string&>(e) << '\n';
        }
        else if(e.type() == typeid(int)){
            std::cout << "int : " << std::any_cast<int>(e) << '\n';
        }
        else if(e.type() == typeid(test)){
            std::cout << "test : " << std::any_cast<const test&>(e).a << '\n';
        }
    }

    accessValue();


    return 0;
}  

