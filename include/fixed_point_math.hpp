#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <type_traits>
#include <algorithm>
#include <string_view>
#include <cmath>
#include <cassert>
#include <fmt/core.h>

struct fixed_point_t_construction_helper{
    std::string_view sv;
    bool negative = false;
};


constexpr inline fixed_point_t_construction_helper operator "" _fixp_t(const char* str){
    fixed_point_t_construction_helper helper{str};
    return helper;
}

constexpr inline fixed_point_t_construction_helper operator-(fixed_point_t_construction_helper helper){
    helper.negative = !helper.negative;
    return helper;
}

constexpr inline fixed_point_t_construction_helper operator+(fixed_point_t_construction_helper helper){
    return helper;
}


template<typename T, size_t fraction>
struct fixed_point_t{
    using int_type = T;
    static_assert(std::is_integral_v<int_type>, "int_type must be an integer type");
    static_assert(fraction <= sizeof(int_type) * 8, "can not have more fractional bits than there are bits in the int");
    
    int_type v;
    
    constexpr fixed_point_t(){
        v = 0;
    }
    
    constexpr fixed_point_t(const fixed_point_t& other){
        v = other.v;
    }
    
    constexpr fixed_point_t& operator=(const fixed_point_t& other){
        v = other.v;
        return *this;
    }
    
    constexpr fixed_point_t(int_type i){
        v = i << frac_bits();
    }
    
    constexpr fixed_point_t(fixed_point_t_construction_helper helper){
        assert(helper.sv.size() != 0);
        
        constexpr auto is_digit = [](char c){return c >= '0' and c <= '9';};
        
        bool before_dot = true;
        int64_t whole = 0;
        int64_t frac = 0;
        int64_t one = 1;
        for(size_t i = 0; i < helper.sv.size(); i++){
            char d = helper.sv[i];
            assert(d == '.' or is_digit(d));
            
            if(d != '.'){
                if(before_dot){
                    int64_t current_digit = static_cast<int64_t>(d - '0');
                    whole *= 10;
                    whole += current_digit;
                    
                }
                else{
                    int64_t current_digit = static_cast<int64_t>(d - '0');
                    frac *= 10;
                    frac += current_digit;
                    one *= 10;
                }
            }
            else{
                before_dot = false;
            }
        }
        
        int64_t result = whole;
        for(size_t i = 0; i < fraction; i++){
            result <<= 1;
            frac *= 2;
            if(frac >= one){
                result |= 1;
                frac -= one;
            }
        }
        
        if(frac * 2 > one){
            result |= 1;
        }
        
        if(helper.negative) result = -result;
        v = result;
    }
    
    constexpr fixed_point_t& operator=(int i){
        v = i << frac_bits();
        return *this;
    }
    
    constexpr bool operator==(const fixed_point_t& other) const{
        return v == other.v;
    }
    
    template<typename S, size_t new_frac_bits>
    explicit constexpr operator fixed_point_t<S, new_frac_bits>() const{
        fixed_point_t<S, new_frac_bits> result;
        if constexpr(new_frac_bits == frac_bits()){
            result.v = static_cast<S>(v);
            return result;
        }
        if constexpr(new_frac_bits < frac_bits()){
            auto difference = frac_bits() - new_frac_bits;
            result.v = static_cast<S>(v >> difference);
            return result;
        }
        else{
            auto difference = new_frac_bits - frac_bits();
            result.v = static_cast<S>(v << difference);
            return result;
        }
        
    }
    
    constexpr static size_t frac_bits(){
        return fraction;
    }
    
    constexpr static size_t whole_bits(){
        size_t result = sizeof(int_type) * 8 - fraction;
        if constexpr(is_signed()){
            result -= 1;
        }
        return result;
    }
    
    constexpr static int_type frac_mask(){
        return (1 << frac_bits())-1;
    }
    
    constexpr static bool is_signed(){
        return std::is_signed_v<int_type>;
    }
    
    constexpr static int_type supremum_as_int(){ //returns the maximum representable value + 1
        return 1 << whole_bits();
    }
    
    constexpr static int_type max_as_int(){
        return (1 << whole_bits()) - 1;
    }
    
    constexpr static int_type minimum_as_int(){
        if constexpr(is_signed()){
            return -supremum_as_int();
        }
        else{
            return 0;
        }
    }
    
    constexpr int_type to_int(){
        return v >> frac_bits();
    }
    
    constexpr int_type whole_part(){
        return (v >> frac_bits()) << frac_bits();
    }
    
    constexpr int_type frac_part(){
        return v & frac_mask();
    }
    
    
    constexpr std::array<char, 12> to_string() const{
        std::array<char, 12> digits = {0};
        size_t idx = 0;
        size_t start = 0;
        
        auto tmp = *this;
        
        if(tmp.v < 0){
            digits[idx++] = '-';
            start++;
            tmp = -tmp;
        }
        
        int_type whole = tmp.whole_part() >> frac_bits();
        int_type frac = tmp.frac_part();
        
        if(whole == 0){
            digits[idx++] = '0';
        }
        else{
            while(whole){
                digits[idx++] = (whole % 10) + '0';
                whole /= 10;
            }
        }
        
        std::reverse(digits.begin()+start, digits.begin() + idx);
        digits[idx++] = '.';
        
        for(; idx < digits.size()-1; idx++){
            frac *= 10;
            digits[idx] = (frac >> frac_bits()) + '0'; 
            frac = frac & frac_mask();
        }
        return digits;
        
        
        
    }
};

template<typename fp_t>
inline void print(const fp_t& f){
    auto arr = f.to_string();
    for(auto c : arr) std::cout << c;
    std::cout << std::endl;
}

template<typename T, size_t fraction>
inline void debug_print(fixed_point_t<T, fraction> f){
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

template<typename T, size_t fraction>
inline constexpr fixed_point_t<T, fraction> fp_from_bits(T i){
    fixed_point_t<T, fraction> result;
    result.v = i;
    return result;
}

template<typename T, size_t fraction>
inline constexpr fixed_point_t<T, fraction> fp_from_float(float f){
    fixed_point_t<T, fraction> result;
    T multiplier = 1 << fixed_point_t<T, fraction>::frac_bits();
    result.v = static_cast<T>(std::round(f * multiplier));
    return result;
}

inline size_t gcd(size_t a, size_t b){
    while(b != 0){
        size_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

template<size_t S>
inline constexpr std::pair<size_t, size_t> reduce_fractional_part(const std::array<char, S>& digits){
    if(digits.size() == 0) return {0, 0};
    size_t a = digits[0] - '0';
    size_t b = 1;
    for(size_t i = 1; i < digits.size(); i++){
        a = a * 10 + digits[i] - '0';
        b = b * 10;
    }
    size_t g = gcd(a, b);
    
    return {a/g, b/g};
}


/*
template<typename fp_t_a, typename fp_t_b>
bool constexpr first_type_is_more_powerful(){
    using int_type_a = typename fp_t_a::int_type;
    using int_type_b = typename fp_t_b::int_type;
    
    constexpr bool type_a_larger_than_type_b = (sizeof(int_type_a) > sizeof(int_type_b));
    constexpr bool type_b_larger_than_type_a = (sizeof(int_type_b) > sizeof(int_type_a));
    
    if(type_a_larger_than_type_b) return true;
    if(type_b_larger_than_type_a) return false;
    
    
}
*/

template<typename T, size_t fraction>
constexpr inline fixed_point_t<T, fraction> operator+(const fixed_point_t<T, fraction> a, const fixed_point_t<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v + b.v);
}

/*
template<typename fp_t_a, typename fp_t_b>
auto operator+(const fp_t_a a, const fp_t_b b){
    
    using int_type_a = typename fp_t_a::int_type;
    using int_type_b = typename fp_t_b::int_type;
    
    constexpr bool type_a_signed = std::is_signed_v<int_type_a>;
    constexpr bool type_b_signed = std::is_signed_v<int_type_b>;
    
    static_assert(type_a_signed == type_b_signed, "don't add ints of different signedness plox");
    
    constexpr bool type_a_larger_than_type_b = (sizeof(int_type_a) > sizeof(int_type_b));
    
    using larger_fp_type = std::conditional_t<type_a_larger_than_type_b, fp_t_a, fp_t_b>;
    
    larger_fp_type tmp_a;
    larger_fp_type tmp_b;
    
    if constexpr(type_a_larger_than_type_b){
        tmp_a = a;
        constexpr auto frac_bits_a = fp_t_a::frac_bits();
        constexpr auto frac_bits_b = fp_t_b::frac_bits();
        if constexpr(frac_bits_a >= frac_bits_b){
            tmp_b.v = b.v;
            tmp_b.v <<= (frac_bits_a - frac_bits_b);
        }
        else{
            tmp_b.v = (b.v >> (frac_bits_b - frac_bits_a));
        }
    }
    else{
        tmp_b = b;
        constexpr auto frac_bits_a = fp_t_a::frac_bits();
        constexpr auto frac_bits_b = fp_t_b::frac_bits();
        if constexpr(frac_bits_b >= frac_bits_a){
            tmp_a.v = a.v;
            tmp_a.v <<= (frac_bits_b - frac_bits_a);
        }
        else{
            tmp_a.v = (a.v >> (frac_bits_a - frac_bits_b));
        }
    }
    return tmp_a + tmp_b;
}
*/

template<typename T, size_t fraction>
constexpr inline fixed_point_t<T, fraction> operator-(const fixed_point_t<T, fraction> a, const fixed_point_t<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v - b.v);
}

template<typename T, size_t fraction>
constexpr inline fixed_point_t<T, fraction> operator-(const fixed_point_t<T, fraction> a){
    return fp_from_bits<T, fraction>(-a.v);
}

template<typename T, size_t fraction>
constexpr inline fixed_point_t<T, fraction> operator*(const fixed_point_t<T, fraction> a, const fixed_point_t<T, fraction> b){
    if constexpr(sizeof(T) <= 4){
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


















