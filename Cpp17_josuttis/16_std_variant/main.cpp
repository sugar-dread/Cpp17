#include <iostream>
#include <variant>
#include <string>
#include <complex>
#include <vector>
#include <set>

/*
    With std::variant<> the C++ standard library provides a new union class, which among other
    benefits supports a new way of polymorphism and dealing with inhomogeneous collections. That is,
    it allows us to deal with elements of different data types without the need for a common base class
    and pointers (raw or smart).

    Variants simply have internal memory for the maximum size of the underlying types plus some
    fixed overhead to manage which alternative is used. No heap memory is allocated.

    Note that empty variants, variants with reference members, variants with C-style array members,
    and variants with incomplete types (such as void) are not allowed.
*/

// std::variant<> Types and Operations
void construction()
{
    // By default, the default constructor of a variant calls the default constructor of the first alternative:
    std::variant<int, int, std::string> v1; // sets first int to 0, index()==0
    
    // If a value is passed for initialization, the best matching type is used:
    std::variant<long, int> v2{42l};  
    std::cout << v2.index() << '\n'; // prints 0
    
    std::variant<long, int> v3{42};
    std::cout << v3.index() << '\n'; // prints 1

    // However, the call is ambiguous if two types match equally well:
    //std::variant<long, long> v4{42}; // ERROR: ambiguous
    //std::variant<int, float> v5{42.3}; // ERROR: ambiguous
    
    std::variant<int, double> v6{42.3}; // OK
    std::variant<int, long double> v7{42.3}; // OK

    // std::variant<std::string, std::string_view> v8{"hello"}; // ERROR: ambiguous
    std::variant<std::string, std::string_view, const char*> v9{"hello"}; // OK
    std::cout << v9.index() << '\n'; // prints 2

    std::cout << "------\n";
    // To pass more than one value for initialization, you have to use the in_place_type or in_place_index tags:
    // std::variant<std::complex<double>> v10{3.0, 4.0}; // ERROR
    // std::variant<std::complex<double>> v11{{3.0, 4.0}}; // ERROR
    std::variant<std::complex<double>> v12{std::in_place_type<std::complex<double>>, 3.0, 4.0};
    std::variant<std::complex<double>> v13{std::in_place_index<0>, 3.0, 4.0};

    std::variant<int, int> v14{std::in_place_index<1>, 77}; // init 2nd int
    std::variant<int, long> v15{std::in_place_index<1>, 77}; // init long, not int
    std::cout << v15.index() << '\n'; // prints 1
    

    // initialize variant with a set with lambda as sorting criterion:
    auto sc = [](int x, int y){
        return std::abs(x) < std::abs(y);
    };

    std::variant<std::vector<int>, std::set<int,decltype(sc)>> v16{std::in_place_index<1>, {4, 8, -7, -2, 0, 5}, sc};

}

void accessValue()
{
    // The usual way to access the value is to call get<>() for the corresponding alternative. You can pass its index or, provided a type is not used more than once, its type. For example:
    std::variant<int, int, std::string> var; // sets first int to 0, index()==0

    // auto a = std::get<double>(var); // compile-time ERROR: no double
    // auto b = std::get<4>(var); // compile-time ERROR: no 4th alternative
    // auto c = std::get<int>(var); // compile-time ERROR: int twice

    try {
        //auto s = std::get<std::string>(var); // throws exception (first int currently set)
        auto i = std::get<0>(var); // OK, i==0
        //auto j = std::get<1>(var); // throws exception (other int currently set)
    }
    catch (const std::bad_variant_access& e) { // in case of an invalid access
        std::cout << "Exception: " << e.what() << '\n';
    }

    if (auto ip = std::get_if<1>(&var); ip) 
    {
        std::cout << *ip << '\n';
    }
    else 
    {
        std::cout << "alternative with index 1 not set\n";
    }
}

void changeTheValue()
{
    std::variant<int, int, std::string> var; // sets first int to 0, index()==0

    var = "hello";

    std::cout << std::get<2>(var) << '\n';  // prints hello

    // You can also use get<>() or get_if<>() to assign a new value to the current alternative:
    std::variant<int, int, std::string> var1; // sets first int to 0, index()==0
    std::get<0>(var1) = 77; // OK, because first int already set
    std::get<1>(var1) = 99; // throws exception (other int currently set)

    if (auto p = std::get_if<1>(&var); p) { // if second int set
        *p = 42; // modify it
    }


}

void comparison()
{
    /*
    For two variants of the same type (i.e., having the same alternatives in the same order) you can use
    the usual comparison operators. The operators act according to the following rules:
    • A variant with a value of an earlier alternative is less than a variant with a value with a later
    alternative.
    • If two variants have the same alternative the corresponding operators for the type of the alternatives
    are evaluated. Note that all objects of type std::monostate are always equal.
    • Two variants with the special state valueless_by_exception() being true are equal. Otherwise,
    any variant with valueless_by_exception() being true is less than any other variant.
    */

    std::variant<std::monostate, int, std::string> v1, v2{"hello"}, v3{42};
    std::variant<std::monostate, std::string, int> v4;

    // v1 == v4 // COMPILE-TIME ERROR
    v1 == v2 // yields false
    v1 < v2 // yields true
    v1 < v3 // yields true
    v2 < v3 // yields false
    v1 = "hello";
    v1 == v2 // yields true
    v2 = 41;
    v2 < v3 // yields true

}

int main()
{
    
    std::variant<int, std::string> var{"hi"}; // initialized with string alternative
    std::cout << var.index() << '\n'; // prints 1
    var = 42;  // now holds int alternative
    std::cout << var.index() << '\n'; // prints 0

    try{  
        // The member function index() can be used to find out, which alternative is currently set (the first alternative has the index 0).
        int i = std::get<0>(var); // access by index
        std::string s = std::get<std::string>(var); // access by type (throws exception in this case)
    }
    catch(const std::bad_variant_access& e){
        std::cerr << "EXCEPTION: " << e.what() << '\n';
    }

    //If there is no default constructor defined for the first type, calling the default constructor for the variant is a compile-time error:
    struct NoDefConstr {
        explicit NoDefConstr(int i) {
            std::cout << "NoDefConstr::NoDefConstr(int) called\n";
        }
    };
    //std::variant<NoDefConstr, int> v1; // ERROR: can’t default construct first type
    
    
    // The auxiliary type std::monostate provides the ability to deal with this situation and also provides the ability to simulate an empty state.
    /*
    std::monostate
    To support variants, where the first type has no default constructor, a special helper type is provided:
    std::monostate. Objects of type std::monostate always have the same state. Thus, they always
    compare equal. Their own purpose is to represent an alternative type so that the variant has no value
    of any other type.
    That is, the struct std::monostate can serve as a first alternative type to make the variant type
    default constructible. For example:
    */
    std::variant<std::monostate, NoDefConstr, int> v2; // OK
    std::cout << "index: " << v2.index() << '\n'; // prints 0

    if (v2.index() == 0) {
        std::cout << "has monostate\n";
    }
    if (!v2.index()) {
        std::cout << "has monostate\n";
    }

    if (std::holds_alternative<std::monostate>(v2)) {
        std::cout << "has monostate\n";
    }

    if (std::get_if<0>(&v2)) {
        std::cout << "has monostate\n";
    }

    if (std::get_if<std::monostate>(&v2)) {
        std::cout << "has monostate\n";
    }

    v2 = 42;
    std::cout << "index: " << v2.index() << '\n'; // index: 1
    v2 = std::monostate{};
    std::cout << "index: " << v2.index() << '\n'; // index: 0


    // Deriving from Variants You can derive from std::variant. For example, you can define an aggregate deriving from a std::variant<> as follows:
    class Derived : public std::variant<int, std::string> {
    };
    
    Derived d = {{"hello"}};
    std::cout << d.index() << '\n'; // prints: 1
    std::cout << std::get<1>(d) << '\n'; // prints: hello
    d.emplace<0>(77); // initializes int, destroys string
    std::cout << std::get<0>(d) << '\n'; // prints: 77


    // construction();

    accessValue();

    std::cout << "--------\n";
    changeTheValue();

    return 0;
}


