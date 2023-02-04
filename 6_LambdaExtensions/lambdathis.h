#include <iostream>
#include <string>
#include <thread>

class Data{
private:
    std::string name;

public:
    explicit Data(const std::string& s) : name{s}{}
    auto startThreadWithCopyOfThis()const{
        // start and return new thread using this after 3 seconds:
        using namespace std::literals;
        
        std::thread t([*this] {
            std::this_thread::sleep_for(3s);
            std::cout << name << '\n';
        });
        return t;
    }
};