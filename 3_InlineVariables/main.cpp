#include <iostream>
#include <map>
#include <filesystem>

int main()
{
    /*
    One strength of C++ is its ability to support the development of header-only libraries. However, up
    to C++17, this was only possible if no global variables/objects were needed or provided by such a library.

    Since C++17 you can define a variable/object in a header file as inline and if this definition is
    used by multiple translation units, they all refer to the same unique object:
    
    class Myclass{
        static inline std::string name = ""; OK since C++17
        ...
    };

    inline Myclass myClobalOng; OK even if included/defined by multiple CPP files
    

    1. Motivation of Inline Variables
    In C++, it is not allowed to initialize a non-const static member inside the class structure:
    
    class MyClass {
        static std::string name = "";  Compile-time ERROR
        ...
    };

    Defining the variable outside the class structure is also an error, if this definitions is part of a header file, included by multiple CPP files:
    class MyClass {
        static std::string name;  OK
        ...
    };
    
    MyClass::name = "";  Link ERROR if included by multiple CPP files
    
    According to the one definition rule (ODR), a variable or entity had to be defined in exactly one translation unit.
    
    Even preprocessor guards do not help:
    #ifndef MYHEADER_HPP
    #define MYHEADER_HPP
    
    class MyClass {
    static std::string name; OK
    ...
    };
    
    MyClass.name = ""; Link ERROR if included by multiple CPP files
    
    #endif

    The problem is not that the header file might be included multiple times, the problem is that two different CPP files include the header so that both define MyClass.name.

    Workarounds

    1- You can initialize static const integral data members in a class/struct:
    class MyClass {
        static const bool trace = false;
        ...
    };


    2- You can define an inline function returning a static local variable:
        inline std::string getName() {
            static std::string name = "initial value";
            return name;
        }

    3- You can define a static member function returning the value:
        std::string getMyGlobalObject() {
            static std::string myGlobalObject = "initial value";
            return myGlobalObject;
        }


    But all these approaches lead to significant overhead, less readability and/or different ways to use the
    global variable. In addition, the initialization of a global variable might be postponed until it’s first usage, which disables applications where we want to initialize objects at program start (such as when using an object to monitor the process).

    Using Inline Variables

    Now, with inline, you can have a single globally available object by defining it only in a header file, which might get included by multiple CPP files:

    class MyClass {
        static inline std::string name = ""; OK since C++17
        ...
    };
    
    inline MyClass myGlobalObj; OK even if included/defined by multiple CPP files

    - It can be defined in multiple translation units, provided all definitions are identical.
    - It must be defined in every translation unit in which it is used.

    You can even apply this to define atomic types in header files only:
    inline std::atomic<bool> ready{false};


    constexpr now implies inline

    For static data members, constexpr implies inline now, such that the following declaration since C++17 defines the static data member n:
    struct D {
        static constexpr int n = 5;  C++11/C++14: declaration, since C++17: definition
    };

    That is, it is the same as:
    struct D {
        inline static constexpr int n = 5;
    };

    */



    return 0;
}