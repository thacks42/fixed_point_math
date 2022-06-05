#pragma once

#include "fixed_point_type.hpp"

namespace fixed_point{

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator+(fixed<T, fraction> a, fixed<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v + b.v);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator+(fixed<T, fraction> a, fixed_construction_helper b){
    fixed<T, fraction> tmp(b);
    return a+tmp;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator+(fixed_construction_helper a, fixed<T, fraction> b){
    fixed<T, fraction> tmp(a);
    return tmp + b;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator+=(fixed<T, fraction>& a, fixed<T, fraction> b){
    a = a+b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator+=(fixed<T, fraction>& a, fixed_construction_helper b){
    a = a+b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(fixed<T, fraction> a, fixed<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v - b.v);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(fixed<T, fraction> a, fixed_construction_helper b){
    fixed<T, fraction> tmp(b);
    return a-tmp;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(fixed_construction_helper a, fixed<T, fraction> b){
    fixed<T, fraction> tmp(a);
    return tmp-b;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator-=(fixed<T, fraction>& a, fixed<T, fraction> b){
    a = a-b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator-=(fixed<T, fraction>& a, fixed_construction_helper b){
    a = a-b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(fixed<T, fraction> a){
    return fp_from_bits<T, fraction>(-a.v);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator*(fixed<T, fraction> a, fixed<T, fraction> b){
    if constexpr(sizeof(T) <= 2){
        if constexpr(std::is_signed_v<T>){
            int32_t result = static_cast<int32_t>(a.v) * static_cast<int32_t>(b.v);
            return fp_from_bits<T, fraction>(result >> fraction);
        }
        else{
            uint32_t result = static_cast<uint32_t>(a.v) * static_cast<uint32_t>(b.v);
            return fp_from_bits<T, fraction>(result >> fraction);
        }
    }
    else if constexpr(sizeof(T) <= 4){
        if constexpr(std::is_signed_v<T>){
            int64_t result = static_cast<int64_t>(a.v) * static_cast<int64_t>(b.v);
            return fp_from_bits<T, fraction>(result >> fraction);
        }
        else{
            uint64_t result = static_cast<uint64_t>(a.v) * static_cast<uint64_t>(b.v);
            return fp_from_bits<T, fraction>(result >> fraction);
        }
    }
    else{
        static_assert(!std::is_same_v<T, T>, "multiplication of 64 bit fixed point numbers is not implemented, sorry");
    }
    
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator*(fixed<T, fraction> a, fixed_construction_helper b){
    fixed<T, fraction> tmp(b);
    return a * tmp;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator*(fixed_construction_helper a, fixed<T, fraction> b){
    fixed<T, fraction> tmp(a);
    return tmp * b;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator*=(fixed<T, fraction>& a, fixed<T, fraction> b){
    a = a*b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator*=(fixed<T, fraction>& a, fixed_construction_helper b){
    a = a*b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> correctly_rounded_division(fixed<T, fraction> a, fixed<T, fraction> b){
    if constexpr(sizeof(T) <= 2){
        if constexpr(std::is_signed_v<T>){
            int32_t x = static_cast<int32_t>(a.v) << fraction;
            int32_t y = static_cast<int32_t>(b.v);
            int32_t result = (x<0) ^ (y<0) ? ((x - y/2)/y) : ((x + y/2)/y);
            return fp_from_bits<T, fraction>(result);
        }
        else{
            uint32_t x = static_cast<uint32_t>(a.v) << fraction;
            uint32_t y = static_cast<uint32_t>(b.v);
            uint32_t result = (x - y/2)/y;
            return fp_from_bits<T, fraction>(result);
        }
    }
    else if constexpr(sizeof(T) <= 4){
        if constexpr(std::is_signed_v<T>){
            int64_t x = static_cast<int64_t>(a.v) << fraction;
            int64_t y = static_cast<int64_t>(b.v);
            int64_t result = (x<0) ^ (y<0) ? ((x - y/2)/y) : ((x + y/2)/y);
            return fp_from_bits<T, fraction>(result);
        }
        else{
            uint64_t x = static_cast<uint64_t>(a.v) << fraction;
            uint64_t y = static_cast<uint64_t>(b.v);
            uint64_t result = (x - y/2)/y;
            return fp_from_bits<T, fraction>(result);
        }
    }
    else{
        static_assert(!std::is_same_v<T, T>, "division of 64 bit fixed point numbers is not implemented, sorry");
    }
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> fast_division(fixed<T, fraction> a, fixed<T, fraction> b){
    if constexpr(sizeof(T) <= 2){
        if constexpr(std::is_signed_v<T>){
            int32_t result = (static_cast<int32_t>(a.v) << fraction) / static_cast<int32_t>(b.v);
            return fp_from_bits<T, fraction>(result);
        }
        else{
            uint32_t result = (static_cast<uint32_t>(a.v) << fraction) / static_cast<uint32_t>(b.v);
            return fp_from_bits<T, fraction>(result);
        }
    }
    else if constexpr(sizeof(T) <= 4){
        if constexpr(std::is_signed_v<T>){
            int64_t result = (static_cast<int64_t>(a.v) << fraction) / static_cast<int64_t>(b.v);
            return fp_from_bits<T, fraction>(result);
        }
        else{
            uint64_t result = (static_cast<uint64_t>(a.v) << fraction) / static_cast<uint64_t>(b.v);
            return fp_from_bits<T, fraction>(result);
        }
    }
    else{
        static_assert(!std::is_same_v<T, T>, "division of 64 bit fixed point numbers is not implemented, sorry");
    }
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator/(fixed<T, fraction> a, fixed<T, fraction> b){
    return fast_division(a,b);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator/(fixed<T, fraction> a, fixed_construction_helper b){
    fixed<T, fraction> tmp(b);
    return a / tmp;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator/(fixed_construction_helper a, fixed<T, fraction> b){
    fixed<T, fraction> tmp(a);
    return tmp / b;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator/=(fixed<T, fraction>& a, fixed<T, fraction> b){
    a = a/b;
    return a;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction>& operator/=(fixed<T, fraction>& a, fixed_construction_helper b){
    a = a/b;
    return a;
}


/*
    y = sqrt(2^n * x)
    y = (2^n * x)^(1/2)
    log2(y) = log2((2^n * x)^(1/2))
    log2(y) = 1/2*log2(2^n * x)
    log2(y) = 1/2*(log2(2^n) + log2(x))
    y = 2^(1/2*(n + log2(x)))
    y ≈ 1<<(1/2*(n + log2(x)))
    y ≈ 1<<(1/2*(n + highest_set_bit(x))
    y ≈ 1<<((n + highest_set_bit(x))/2)
*/
template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> approx_sqrt(fixed<T, fraction> x){
    int leading_zeros = std::countl_zero(static_cast<std::make_unsigned_t<T>>(x.v));
    int highest_set_bit = (sizeof(T) * 8) - leading_zeros - 1;
    return fp_from_bits<T, fraction>(1 << ((highest_set_bit + fraction)/2));
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> better_approx_sqrt(fixed<T, fraction> x){
    int leading_zeros = std::countl_zero(static_cast<std::make_unsigned_t<T>>(x.v));
    int highest_set_bit = (sizeof(T) * 8) - leading_zeros - 1;
    auto initial_guess = fp_from_bits<T, fraction>(1 << ((highest_set_bit + fraction)/2));
    int shift_amount = (highest_set_bit + static_cast<int>(fraction))/2 - static_cast<int>(fraction);
    x.v = shift_amount >= 0 ? (x.v >> shift_amount) : x.v << (-shift_amount);
    initial_guess = initial_guess + x;
    initial_guess.v >>= 1;
    return initial_guess;
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> sqrt(fixed<T, fraction> x){
    auto y = better_approx_sqrt(x);
    for(int i = 0; i < 4; i++){ //TODO: this could be made more accurate. Fix it pls!
        y = y + x/y;
        y.v >>= 1;
    }
    y = y + correctly_rounded_division(x,y);
    y = correctly_rounded_division(y, make_fixed<T, fraction>(2));
    return y;
}


template<typename T, size_t fraction>
constexpr inline bool same_top_most_bit(fixed<T, fraction> a, fixed<T, fraction> b){
    int leading_zeros_a = std::countl_zero(static_cast<std::make_unsigned_t<T>>(a.v));
    int leading_zeros_b = std::countl_zero(static_cast<std::make_unsigned_t<T>>(b.v));
    return leading_zeros_a == leading_zeros_b;
}

template<typename T, size_t fraction>
constexpr inline bool same_up_to_one_bit(fixed<T, fraction> a, fixed<T, fraction> b){
    return std::abs(a.v - b.v) <= 1;
}

template<typename T, size_t fraction>
constexpr inline bool same_up_to_n_bits(fixed<T, fraction> a, fixed<T, fraction> b, int n){
    return std::abs(a.v - b.v) <= (1<<n);
}

template<typename T, size_t fraction>
inline constexpr std::strong_ordering operator<=>(fixed<T, fraction> a, fixed<T, fraction> b){
    return a.v <=> b.v;
}


template<typename T, size_t fraction>
inline constexpr std::strong_ordering operator<=>(fixed<T, fraction> a, fixed_construction_helper b){
    fixed<T, fraction> tmp(b);
    return a.v <=> tmp.v;
}

template<typename T, size_t fraction>
inline constexpr std::strong_ordering operator<=>(fixed_construction_helper a, fixed<T, fraction> b){
    fixed<T, fraction> tmp(a);
    return tmp.v <=> b.v;
}


template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> abs(fixed<T, fraction> a){
    fixed<T, fraction> result;
    result.v = std::abs(a.v);
    return result;
}

}//end namespace fixed_point












