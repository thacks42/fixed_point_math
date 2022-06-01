#pragma once

#include "fixed_point_math.hpp"
#include <iostream>
#include <fmt/core.h>
namespace fixed_point{

template<typename T, size_t fraction>
inline void print(const fixed<T, fraction>& f){
    auto arr = f.to_string();
    for(auto c : arr) std::cout << c;
    std::cout << "\n";
}


template<typename T, size_t fraction>
inline void debug_print(fixed<T, fraction> f){
    double d = f.to_int();
    double frac = 0.5;
    T frac_part = f.frac_part();
    for(size_t i = 0; i < fraction; i++){
        if((frac_part >> (fraction - i - 1)) & 1){
            d += frac;
        }
        frac /= 2.0;
    }
    fmt::print("binary: {:>{}b}.{:0>{}b} as double: {:>20.12f}\n", f.to_int(), f.whole_bits(), f.frac_part(), f.frac_bits(), d);
}

}//namespace fixed_point
