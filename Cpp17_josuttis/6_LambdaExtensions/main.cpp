#include <iostream>
#include "lambdathis.h"

/*
C++17 improved their abilities to allow the use of lambdas in even more places:
    • in constant expressions (i.e., at compile time)
    • in places where you need a copy of the current object (e.g., when calling lambdas in threads)
*/

int main()
{
    /* constexpr Lambdas
    Since C++17, lambdas are implicitly constexpr if possible. That is, any lambda can be used in
    compile-time contexts provided the features it uses are valid for compile-time contexts (e.g., only
    literal types, no static variables, no virtual, no try/catch, no new/delete).

    To find out at compile time whether a lambda is valid for a compile-time context, you can declare it as constexpr:
    */

    auto squared = [](auto val) constexpr{return val*val;}; // OK since C++17
    auto squared1 = [](auto val) constexpr -> int {return val*val;}; // OK since C++17

    auto squared2 = [](auto val) constexpr -> int {
            //static int calls = 0; // ERROR: static variable in compile-time context
            return val*val;
        }; // OK since C++17


    /*
    Passing Copies of this to Lambdas
    When using lambdas in member functions, you have no implicit access to the object the member
    function is called for. That is, inside the lambda, without capturing this in any form, you can’t use
    members of the object (independent from whether you qualify them with this->):
    */

    class C{
    public:
        void foo(){
            //auto l1 = [] {std::cout << name << '\n';};          //ERROR
            //auto l2 = [] {std::cout << this->name << '\n';};    //ERROR
        }

        // Since C++17, you can explicitly ask to capture a copy of the current object by capturing *this:
        // That is, the capture *this means that a copy of the current object is passed to the lambda.
        void foo1(){
            auto l1 = [*this] {std::cout << name << '\n';};          //OK
        }
    private:
        std::string name;
    };

    std::thread t;
    {
        Data d{"c1"};
        t = d.startThreadWithCopyOfThis();
    } // d is no longer valid
    t.join();


    /*
    The lambda takes a copy of *this, which means that a copy of d is passed. Therefore, it is no
    problem that probably the thread uses the passed object after the destructor of d was called.
    If we’d have captured this with [this], [=], or [&], the thread runs into undefined behavior,
    because when printing the name in the lambda passed to the thread the lambda would use a member
    of a destroyed object.
    */


    return 0;
}