#include <iostream>

#include "test_arithmetic.hpp"
#include "test_ctor.hpp"

int main(){
    bool all_passed = true;
    all_passed &= test_arithmetic();
    all_passed &= test_ctor();
    
    
    if(!all_passed){
        std::cerr << "Failed test suite!!!" << std::endl;
        return 1;
    }
    else{
        std::cout << "All tests passed!" << std::endl;
        return 0;
    }
}
