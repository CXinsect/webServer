#include <iostream>
#include <vector>
#include <boost/any.hpp>
#include <boost/algorithm/string.hpp>
int main(void) {
    auto a = boost::any(1);
    std::cout << "int" << boost::any_cast<int>(a) << std::endl;
    try {
        std::cout << boost::any_cast <std::string>(a) << std::endl;
    }
    catch (const boost::bad_any_cast& e) {
        std::cout << e.what() << std::endl;
    }
    try {
        if(int*a = boost::any_cast<int*>(&a))
        std::cout << "int* " << *a << std::endl;
    else {
            try {
                if(std::string* a = boost::any_cast<std::string*>(&a)) 
                    std::cout << "string" << std::endl;
                else {
                    std::cout << "Other Types" << std::endl;
                }
            } catch(const boost::bad_any_cast& e) {
                std::cout << e.what() << std::endl;
            }
        
        }
    }
        catch(const boost::bad_any_cast& e) {
            std::cout << e.what() << std::endl;
        }
    return 0;
}