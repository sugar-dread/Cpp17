#include <iostream>

int main()
{
    /*
    Attribute [[nodiscard]]
    The new attribute [[nodiscard]] can be used to encourage warnings by the compiler if a return
    value of a function is not used (issuing a warning is not required, though).
    Usually, this can be used to signal misbehavior when return values are not used. The misbehavior
    might be:
        • memory leaks, such as not using returned allocated memory,
        • unexpected or non-intuitive behavior, such as getting different/unexpected behavior when not
        using the return value;
        • unnecessary overhead such as calling something that is a no-op if the return value is not used.
    */
    class Mycontainer{
        public:
            [[nodiscard]] bool empty() const noexcept;
    };

    /*
    Attribute [[maybe_unused]]
    The new attribute [[maybe_unused]] can be used to avoid warnings by the compiler for not using
    a name or entity.
    The attribute may be applied to the declaration of a class, a type definition with typedef or using,
    a variable, a non-static data member, a function, an enumeration type, or an enumerator (enumeration
    value).
    */

    void foo(int val, [[maybe_unused]] std::string msg)
    {
        #ifdef DEBUG
            log(msg);
        #endif
    }

    class Mystruct{
        char c;
        int i;
        [[maybe_unused]] char makeLargerSize[100];
    };
    /*
    Note that you can’t apply [[maybe_unused]] to a statement. For this reason, you cannot counter
    [[nodiscard]] with [[maybe_unused]] directly:1
    */
    [[nodiscard]] void* foo();
    
    foo(); // WARNING: return value not used
    [[maybe_unused]] foo(); // ERROR: attribute not allowed here
    [[maybe_unused]] auto x = foo(); // OK


    /*
    Attribute [[fallthrough]]
    The new attribute [[fallthrough]] can be used to avoid warnings by the compiler for not having
    a break statement after a sequence of one or more case labels inside a switch statement.
    */
    void commentPlace(int place)
    {
        switch (place) {
            case 1:
                std::cout << "very ";
                [[fallthrough]];
            case 2:
                std::cout << "well\n";
                break;
            default:
                std::cout << "OK\n";
                break;
            }
    }

    return 0;
}


