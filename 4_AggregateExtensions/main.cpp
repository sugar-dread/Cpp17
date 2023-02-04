#include <iostream>

int main()
{

    // One way to initialize objects in C++ is aggregate initialization, which allows the initialization of an aggregate from multiple values with curly braces:
    struct Data{
        std::string name;
        double value;
    };

    Data x{"test1", 6.778};

    //Since C++17, aggregates can have base classes, so that for such structures being derived from other classes/structures list initialization is allowed:
    struct MoreData : Data{
        bool done;
    };

    MoreData y{{"test1", 6.778}, false};

    // As you can see, aggregate initialization now supports nested braces to pass values to the derived members of the base class.
    // And as for initialization of subobjects with members, you can skip the nested braces if a base type or subobject only gets one value:

    MoreData z{"test1", 6.778, false};

    /*
    Motivation for Extended Aggregate Initialization
    Without this feature, deriving a structure from another disabled aggregate initialization, so that you
    had to define a constructor:
    struct Cpp14Data : Data {
        bool done;
        Cpp14Data (const std::string& s, double d, bool b)
            : Data{s,d}, done{b} {
        }
    };

    Cpp14Data y{"test1", 6.778, false};
    
    Now we have this ability for free with the syntax using nested braces, which can be omitted if only
    one value is passed:
    MoreData x{{"test1", 6.778}, false}; // OK since C++17
    MoreData y{"test1", 6.778, false}; // OK
    Note that because this is an aggregate now, other initializations are possible:
    MoreData u; // OOPS: value/done are uninitialized
    MoreData z{}; // OK: value/done have values 0/false
    If this is too dangerous, you still better provide a constructor.
    */

    return 0;
}