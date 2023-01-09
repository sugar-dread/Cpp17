#include <iostream>
#include <string>
#include <optional>
#include <string_view>
#include <charconv> // for from_chars()

/*
    With C++17, a special string class was adopted by the C++ standard library, that allows us to deal
    with character sequences like strings, without allocating memory for them: std::string_view.
    That is, std::string_view objects refer to external character sequences without owning them.
    That is, the object can be considered as a reference to a character sequence.

    Using such a string view is cheap and fast (passing a string_view by value is always cheap).
    However, it is also potentially dangerous, because similar to raw pointers it is up to the programmer
    to ensure that the referred character sequences is still valid, when using a string_view).

    Differences to std::string
    In contrast to std::string, std::string_view objects have the following properties:
        • The underlying character sequences are read-only. There are no operations that allow the modification
        of the characters. You can only assign a new value, swap values, and remove characters at
        the beginning or the end.
        • The character sequences are not guaranteed to be null terminated. So, a string view is not a null terminated byte stream (NTBS).
        • The value can be the nullptr, which for example is returned by data() after initializing a stringview with the default constructor.
        • There is no allocator support.
        Due to the possible nullptr value and possible missing null terminator, you should always use size() before accessing characters via operator[] or data() (unless you know better).
    
    There are two major applications of string views:
        1. You might have allocated or mapped data with character sequences or strings and want to use this data without allocating more memory. Typical examples are the use of memory mapped files or dealing with substrings in large texts.
        2. You want to improve the performance for functions/operations that receive strings just to directly process them read-only not needing a trailing null terminator.



    Summary of Safe Usage of String Views
    To summarize, use std::string_view with care, which mean that you should also change the
    general style you program:
    • Don’t use string views in API’s that pass the argument to a string.
    – Don’t initialize string members from string view parameters.
    – No string at the end of a string view chain.
    • Don’t return a string view.
    – Unless it is just a forwarded input argument or you signal the danger by, for example, naming
    the function accordingly.
    • For this reason, function templates should never return the type T of a passed generic argument.
    – Return auto instead.
    • Never use a returned value to initialize a string view.
    • For this reason, don’t assign the return value of a function template returning a generic type to
    auto.
    – This means, the AAA (Almost Always Auto) pattern is broken with string view.
    If these rules are too complicated or hard to follow, don’t use std::string_view at all (unless you
    know what you do).

    Using String Views in API’s

    First, using a std::string_view only makes sense if the function using the parameter has the
    following constraints:
    • It doesn’t expect a null terminator at the end. This, for example, is not the case when passing the
    argument to a C functions as a single const char*.
    • It respects the lifetime of the passed argument. Usually this means that the receiving function uses
    the passed value only until it ends.
    • The calling function should not deal with the owner of the underlying characters (such as deleting
    it, changing its value, or freeing its memory).
    • It can deal with the nullptr as value.

*/

template<typename T>
void printElems(const T& coll, std::string_view prefix = std::string_view{})
{
    for(const auto& elem : coll)
    {
        if(prefix.data()){ // check against nullptr
            std::cout << prefix << ' ';
        }
        std::cout << elem << '\n';
    }

    /*
        template<typename T>
        void printElems(const T& coll, const std::string& prefix = std::string{});
        and we pass a string literal, the call creates a temporary string which will allocate memory unless the
        string is short and the short string optimization is used. By using a string view instead, no allocation
        is needed, because the string view only refers to the string literal.
    */
}

// convert string to int if possible:
std::optional<int> asInt(std::string_view sv)
{
    int val;

    auto [ptr, ec] = std::from_chars(sv.data(), sv.data()+sv.size(), val);

    // if we have an error code, return no value:
    if(ec != std::errc{})
    {
        return std::nullopt;
    }
    return val;
}


void construction()
{
    /*
    You can create a string view with the default constructor, as a copy, from a raw character array (null terminated or with specified length), from a std::string, or as a literal with the suffix sv. However, note the following:
    • String views created with the default constructor have nullptr as data(). Thus, there is no valid call of operator[].
    */

    std::string_view sv;
    auto p = sv.data(); // yields nullptr
    //std::cout << sv[0]; // ERROR: no valid character

    std::string_view sv1{"hello"};
    std::cout << sv1 << '\n'; // OK
    std::cout << sv1.size() << '\n'; // 5
    // std::cout << sv1.at(5); // throws std::out_of_range exception
    std::cout << sv1[5] << '\n';  // undefined behavior, but HERE it usually works
    std::cout << sv1.data() << '\n'; // "hello"

    std::string_view sv2{"hello", 6}; // NOTE: 6 to include '\0'
    std::cout << sv2.size() << '\n'; // 6
    std::cout << sv2.at(5) << '\n'; // OK, prints the value of '\0'
    std::cout << sv2[5] << '\n'; // OK, prints the value of '\0'
    std::cout << sv2.data() << '\n'; // OK

    /*
    • To create a string view from a std::string an implicit conversion operator is provided in class
    std::string. Again, having the null terminator right after the last character, which is usually
    guaranteed for a string, is not guaranteed to exist for the string view:
    */
    std::string s = "hello";
    std::cout << s.size() << '\n'; // 5
    std::cout << s.at(4) << '\n'; // exceptiom throw
    std::cout << s[5] << '\n'; // OK, prints the value of '\0'
    
    std::string_view sv3{s};
    std::cout << sv3.size() << '\n'; // 5
    //std::cout << sv3.at(5) << '\n'; // throws std::out_of_range exception
    std::cout << sv3[5] << '\n'; // undefined behavior, but HERE it usually works
    std::cout << sv3.data() << '\n'; // undefined behavior, but HERE it usually works

}

// Modifying a String View
// There are only a few operations provided to modify a string view:
void modifyStringView()
{
    std::string_view sv1 = "hey";
    std::string_view sv2 = "world";
    sv1.swap(sv2);
    std::cout << sv1.data() << '\n';
    std::cout << sv2.data() << '\n';

    sv2 = sv1;
    std::cout << sv1.data() << '\n';
    std::cout << sv2.data() << '\n';

    std::string_view sv = "I like my kindergarten";
    sv.remove_prefix(2);
    sv.remove_suffix(8);
    std::cout << sv << "\n"; // prints: like my kind


    //Note that there is no support for operator+. Thus:
    std::string_view sv3 = "hello";
    std::string_view sv4 = "world";
    // auto s1 = sv1 + sv2; // ERROR
    
    //One of the operands has to be a string:
    auto s2 = std::string(sv3) + std::string(sv4); // OK
    
    // Note that there is no implicit conversion to a string, because this is an expensive operation because it
    // might allocate memory. For this reason, only the explicit conversion is possible.
    // There is also in implicit conversion from a string to a string view.

}

void initializeStrings()
{
    // 1- consider the "good old way" to initialize a string member. It has drawbacks. Initializing a person with a string litera cerates one unnecessary
    // copy, which might cause an unnecessary request for heap memory. Because of reference type, std::string constructorcreates the temporary parameter n.
    // this means it allocates heap memory. Even with move semantics the temporary string is then copied to initialize the member name, which means that again memory is allocated.
    
    class Person{
    public:
        explicit Person(const std::string& n) : name(n){}
    private:
        std::string name;
    };


    // 2- this performance is better than above
    // Now a temporary string view n gets created, which does not allocate memory at all, because the string view only refers to the characters of the string literal. Only the initialization of name allocates
    // once the memory for the member name.
    // however there is a problem, if you pass a temporary string or a string marked with std::move(), the string is converted to type a string view(which is cheap)
    // and then the string view is used to allocate the memory for the new string(which is expensive). the use of string view disables move semantics unless
    // you provide an additional overload for it.

    class Person2{
    public:
        explicit Person2(std::string_view n) : name(n){}
    private:
        std::string name;
    };

    // 3- There is still a clear recommendation for how to initialize objects with string members: Take the string by value and move:
    // we anyway have to create a string. We only have "move" which is a cheap operation
    class Person3{
    public:
        explicit Person3(std::string n) : name(std::move(n)){}
    private:
        std::string name;
    };

    /*
    This example again demonstrates:
    • String views are not a better interface for taking strings.
    • In fact, string views should only be used in call chains, where they never have to be used as strings.
    */
}


void usingStringViewsInsteadOfString()
{
    std::string toString(const std::string& prefix, const std::chrono::system_clock::time_point& tp)
    {
        auto rawtime = std::chrono::system_clock::to_time_t(tp);
        std::string ts = std::ctime(&rawtime); // mote mot thread safe

        ts.resize(ts.size()-1);

        return prefix + ts;
    }

    // you could implement the following:
    std::string toString(std::string_view prefix, const std::chrono::system_clock::time_point& tp)
    {
        auto rawtime = std::chrono::system_clock::to_time_t(tp);

        std::string_view ts = std::ctime(&rawtime); // NOTE: not thread safe
        ts.remove_suffix(1); // skip trailing newline
        

        return std::string(prefix) + ts; // unfortunately no operator + yet

        /*
        Note that we can remove the trailing newline from the string, but that we can’t concatenate both string views
        by simply calling operator+. Instead, we have to convert one of the operands to a std::string
        (which unfortunately unnecessarily might allocate additional memory).
        */
    }

}

int main()
{
    for(auto s : {"42", " 077", "hello", "0x33"}){
        // try to convert s to int and print the result if possible:
        std::optional<int> oi = asInt(s);
        if (oi) {
            std::cout << "convert '" << s << "' to int: " << *oi << "\n";
        }
        else {
            std::cout << "can't convert '" << s << "' to int\n";
        }
    }
    /*
        Now, asInt() takes a string view by value. However, that has significant consequences. First, it
        does no longer make sense to use std::stoi() to create the integer, because stoi() takes a string
        and creating a string from a string view is a relative expensive operation.
    */



    construction();
    std::cout << "-----------------\n";
    modifyStringView();

    return 0;
}  

