#pragma once

#include <iostream>
#include <string>
#include <chrono>

/********************************************
* timer to print elapsed time
********************************************/

class Timer
{
public:
    Timer(){}
    void printDiff(const std::string& msg = "Timer diff :")
    {
        auto now{std::chrono::steady_clock::now()};
        std::chrono::duration<double, std::milli> diff{now-last};
        std::cout << msg << diff.count() << "ms\n";
        last = std::chrono::steady_clock::now();
    }

private:
    std::chrono::steady_clock::time_point last{std::chrono::steady_clock::now()};
};