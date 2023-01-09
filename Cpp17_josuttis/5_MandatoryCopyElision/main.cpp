#include <iostream>

class Myclass{
public:
    Myclass()
    {
        std::cout << "Ctor\n";
    }

    ~Myclass()
    {
        std::cout << "Dtor\n";
    }

    Myclass(const Myclass&)
    {
        std::cout << "Copy Ctor\n";
    }

    Myclass& operator=(const Myclass&)
    {
        std::cout << "Copy Assignement\n";
        return *this;
    }

    Myclass(Myclass&&)noexcept
    {
        std::cout << "Move Ctor\n";
    }
    
    Myclass& operator=(Myclass&&)noexcept
    {
        std::cout << "Move Assignement\n";
        return *this;
    }

};



void foo(Myclass param) // param is initialized by passed argument
{
    std::cout << "foo()\n";
}



Myclass bar(){
    return Myclass(); // returns temporary
}


class CopyOnly{
public:
    CopyOnly(){}
    CopyOnly(int){}
    CopyOnly(const CopyOnly&) = default;
    CopyOnly& operator=(const CopyOnly&) = delete;  //explicitly deleted
    CopyOnly(CopyOnly&&) = delete; //explicitly deleted
};

CopyOnly ret() {
    return CopyOnly{}; // OK since C++17
}

int main()
{
    // Motivation for Mandatory Copy Elision for Temporaries

    Myclass a;
    foo(a);  // burada hem ctor hem copy ctor çağrıldı
    std::cout << "-------\n"; 

    foo(Myclass()); // pass temporary to initialize param. In here ctor is called, no copy constructor is called.
    Myclass x = bar(); // use returned temporary to initialize x In here ctor is called, no copy constructor is called.
    foo(bar()); // use returned temporary to initialize param In here ctor is called, no copy constructor is called.

    /*
    However, because these optimizations were not mandatory, copying the objects had to be possible
    by providing an implicit or explicit copy or move constructor. That is, although the copy/move
    constructor was usually not called, it had to exist. Code like this didn’t compile when no copy/move
    constructor was defined.
    The copy elision to initialize objects from temporaries is mandatory since C++17 
    
    In fact, what we will see later is that we simply pass a value for initialization as argument or return value that is
    used then to materialize a new object.
    
    When we delete move ctor, still it works because copy elision does not need move ctor.
    */

    CopyOnly y = 42; // OK since C++17
    /*
    The initialization of x was invalid before C++17, because the copy initialization (initialization using
    the =) needed the conversion of 42 to a temporary and that temporary in principle needed the move
    constructor, although it was never called. (The fact that the copy constructor serves as fallback for a
    move constructor only applies, if the move constructor is not user-declared.)
    */


    return 0;
}


/*
VALUE TYPE IN C++
Examples of lvalues are:
    • An expression that is just the name of a variable, function, or member
    • An expression that is just a string literal
    • The result of the built-in unary * operator (i.e., what dereferencing a raw pointer yields)
    • The result of a function returned by lvalue reference (type&)
Examples of prvalues are:
    • Expressions that consist of a literal that is not a string literal (or a user-defined literal, where the return type of the associated literal operator defines the category)
    • The result of the built-in unary & operator (i.e., what taking the address of an expression yields)
    • The result of built-in arithmetic operators
    • The result of a function returned by value
    • A lambda expression
Examples of xvalues are:
    • The result of a function returned by rvalue reference (type&&, especially returned by std::move())
    • A cast to an rvalue reference to an object type
Roughly speaking:
    • All names used as expressions are lvalues.
    • All string literals used as expression are lvalues.
    • All other literals (4.2, true, or nullptr) are prvalues.
    • All temporaries (especially objects returned by value) are prvalues.
    • The result of std::move() is an xvalue.




*/