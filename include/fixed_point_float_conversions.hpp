#pragma once

#include "fixed_point_math.hpp"
#include <cmath>

template<typename T, size_t fraction>
inline constexpr fixed_point::fixed<T, fraction> fp_from_float(float f){
    fixed_point::fixed<T, fraction> result;
    T multiplier = 1 << fixed_point::fixed<T, fraction>::frac_bits();
    result.v = static_cast<T>(std::round(f * multiplier));
    return result;
}
