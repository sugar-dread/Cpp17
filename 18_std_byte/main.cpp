#include <iostream>
#include <string>
#include <any>
#include <vector>
#include <complex>
#include <set>
#include <cstddef>

/*
std::byte
    Programs hold data in memory. With std::byte C++17 introduces a type for it, which does represent
    the “natural” type of the elements of memory, bytes. The key difference to types like char
    or int is that this type cannot (easily) be (ab)used as an integral value or character type. For cases,
    where numeric computing or character sequence are not the goal, this results in more type safety.
    The only “computing” operations supported are bit-wise operators.
*/

int main()
{
    std::byte b1{0x3F};
    std::byte b2{0b1111'0000};

    std::byte b3[4] {b1, b2, std::byte{1}}; // 4 bytes (last is 0)


    if (b1 == b3[0]) {
        b1 <<= 1;
    }
    std::cout << std::to_integer<int>(b1) << '\n'; // outputs: 126

    // Note that list initialization (using curly braces) is the only way you can directly initialize a single value of a std::byte object. All other forms do not compile:
    // This is a direct consequence of the fact that std::byte is implemented as an enumeration type, using the new way scoped enumerations can be initialized with integral values.
    
    
    std::byte b5{42}; // OK (as for all enums with fixed underlying type since C++17)
    //std::byte b2(42); // ERROR
    //std::byte b3 = 42; // ERROR
    //std::byte b4 = {42}; // ERROR

    // There is also no implicit conversion, so that you have to initialize the byte array with an explicitly converted integral literal:
    // std::byte b5[] {1}; // ERROR
    std::byte b6[] {std::byte{1}}; // OK
    
    // Without any initialization, the value of a std::byte is undefined for object on the stack:
    // std::byte b; // undefined value

    // As usual (except for atomics), you can force an initialization with all bits set to zero with list initialization:
    std::byte b{}; // same as bf0g

    //Such a conversion is also necessary to use a std::byte as a Boolean value. For example:
    // if (b2 // ERROR
    if (b2 != std::byte{0})  // OK
    //if (to_integer<bool>(b2))  // ERROR (ADL doesn’t work here)
    if (std::to_integer<bool>(b2))  // OK

    return 0;
}  

