#include <iostream>
#include <optional>
#include <string>
#include <complex>


std::optional<int> asInt(const std::string& s)
{
    try{
        return std::stoi(s);
    }
    catch(...)
    {
        return std::nullopt;
    }
}

class Name
{
public:
    Name(std::string f, std::optional<std::string> m, std::string l)
        : first{std::move(f)}, middle{std::move(m)}, last{std::move(l)}{}    
    
    friend std::ostream& operator<< (std::ostream& strm, const Name& n)
    {
        strm << n.first << ' ';
        if(n.middle.has_value()){
            strm << n.middle.value() << ' ';
        }
        return strm << n.last;
    }

private:
    std::string first;
    std::optional<std::string> middle;
    std::string last;
};

int main()
{
    /*
    std::optional<>
    Optional objects simply have internal memory for the contained objects plus a Boolean flag. Thus,
    the size usually is one byte larger than the contained object. For some contained types, there might
    even be no size overhead at all, provided the additional information can be placed inside the contained
    object. No heap memory is allocated. The objects use the same alignment as the contained type.


    Using std::optional<>
    std::optional<> model a nullable instance of an arbitrary type. The instance might be a member,
    an argument, or a return value. You could also argue that a std::optional<> is a container for zero
    or one element.
    */
    for(auto s : {"42", " 077", "hello", "0x33"}){
        std::optional<int> oi = asInt(s);
        if(oi){
            std::cout << "convert " << s << " to int: " << *oi << "\n";
        }
        else {
            std::cout << "can't convert '" << s << "' to int\n";
        }

        if (oi.has_value()) {
            std::cout << "convert '" << s << "' to int: " << oi.value() << "\n";
        }
    }


    /*
    Optional Arguments and Data Members
    Another example using std::optional<> is the optional passing of arguments and/or the optional
    setting of a data member:
    */

    Name n{"Jim", std::nullopt, "Knopf"};
    std::cout << n << '\n';
    Name m{"Donald", "Ervin", "Knuth"};
    std::cout << m << '\n';


    // std::optional<> Operations
    // Construction
    std::optional<int> o1;
    std::optional<int> o2(std::nullopt);
    std::optional o3{42}; // deduces optional<int>
    std::optional<std::string> o4{"hello"};
    std::optional o5{"hello"}; // deduces optional<const char*>
    std::optional<std::string> o9{o5}; // OK copy optional objects

    auto o10 = std::make_optional(3.0); // optional<double>
    auto o11 = std::make_optional("hello"); // optional<const char*>
    auto o12 = std::make_optional<std::complex<double>>(3.0, 4.0);


    // Accessing the Value
    std::optional o{42};
    if (o){}  // true
    if (!o){}  // false
    if (o.has_value()){}  // true

    std::optional o13{std::pair{42, "hello"}};
    auto p = *o13; // initializes p as pair<int,string>
    std::cout << o13->first; // prints 42
    std::cout << o.value_or(15); // OK (outputs fallback if no value)


    std::optional<int> o0;
    std::optional<int> o14{42};
    
    o0 == std::nullopt; // yields true
    o0 == 42; // yields false
    o0 < 42; // yields true
    o0 > 42; // yields false
    o14 == 42; // yields true
    o0 < o14; // yields true

    //Again, implicit type conversions for the underlying type are supported:
    std::optional<int> o15{42};
    std::optional<double> o16{42.0};

    o15 == 42; // yields true
    o16 == o15; // yields true

    std::optional<std::complex<double>> o17; // has no value
    std::optional ox{77}; // optional<int> with value 77
    o17 = 42; // value becomes complex(42.0, 0.0)
    o17 = {9.9, 4.4}; // value becomes complex(9.9, 4.4)
    o17 = ox; // OK, because int converts to complex<double>
    o17 = std::nullopt; // o no longer has a value
    o17.emplace(5.5, 7.7); // value becomes complex(5.5, 7.7)

    o.reset(); // o no longer has a value

    /*
    std::optional<> also supports move semantics. If you move the object as a whole, the state gets
    copied and the contained object (if any) is moved. As a result, a moved-from object still has the same
    state, but any value becomes unspecified.
    */

    std::optional<std::string> os;
    std::string s = "a very very very long string";
    os = std::move(s); // OK, moves
    std::string s2 = *os; // OK copies
    std::string s3 = std::move(*os); // OK, moves

    if(os.has_value()){std::cout << os.value() << "xccc\n";}

    return 0;
}


