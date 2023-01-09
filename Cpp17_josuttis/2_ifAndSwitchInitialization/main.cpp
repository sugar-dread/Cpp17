#include <iostream>
#include <map>
#include <filesystem>

int main()
{
    
    // if with Initialization
    // Any value initialized inside an if statement is valid until the end of the then and the else part (if any).

    if(std::string s{"123456"}; s.size() > 5)
    {
        std::cout << s << " is greater than 5\n";
    }
    else
    {
        std::cout << s << " is less than 5\n";
    }
    // s is no longer declared in here. The destructor for s is called at the end of the then-part or at the end of the else-part.
    // Note that any object being initialized must have a name. Otherwise, the initialization creates and immediately destroys a temporary.

    std::map<std::string, int> coll;
    coll.insert({"new", 42});

    if(auto [pos, ok] = coll.insert({"new", 42}); !ok)
    {
        const auto& [key, val] = *pos;
        std::cout << "already there : " << key << "\n";
    }

    // switch with Initialization
    switch(std::filesystem::path p("/home/phytm/Desktop"); std::filesystem::status(p).type()) {
        case std::filesystem::file_type::not_found:
            std::cout << p << " not found\n";
            break;
        case std::filesystem::file_type::directory:
            std::cout << p << ":\n";
            for (auto& e : std::filesystem::directory_iterator(p)) {
                std::cout << "- " << e.path() << '\n';
            }
            break;
        default:
            std::cout << p << " exists\n";
            break;
    } 

    return 0;
}