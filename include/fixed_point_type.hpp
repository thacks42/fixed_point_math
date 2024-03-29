#pragma once

#include <cstdint>
#include <bit>
#include <array>
#include <type_traits>
#include <cassert>
#include <algorithm>
#include <compare>

namespace fixed_point{

template<size_t S>
struct fixed_construction_helper{
    std::array<char, S> str;
    int64_t whole;
    uint64_t frac;
    bool negative;
};

template<char... str>
consteval inline auto operator ""_fixp_t(){
    fixed_construction_helper<sizeof...(str)> helper{{str...}, false, 0, 0};
    assert(helper.str.size() != 0);
    
    constexpr auto is_digit = [](char c){return c >= '0' and c <= '9';};
    
    bool before_dot = true;
    int64_t whole = 0;
    uint64_t frac = 0;
    uint64_t one = 1;
    for(size_t i = 0; i < helper.str.size(); i++){
        char d = helper.str[i];
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
    uint64_t bin_frac = 0;
    for(int i = 0; i < 64; i++){
        bin_frac <<= 1;
        frac <<= 1;
        if(frac >= one){
            bin_frac |= 1;
            frac -= one;
        }
    }

    helper.whole = whole;
    helper.frac = bin_frac;
    return helper;
}

template<size_t S>
constexpr inline fixed_construction_helper<S> operator-(fixed_construction_helper<S> helper){
    helper.negative = !helper.negative;
    return helper;
}

template<size_t S>
constexpr inline fixed_construction_helper<S> operator+(fixed_construction_helper<S> helper){
    return helper;
}


template<typename T, size_t fraction>
struct fixed{
    using int_type = T;
    static_assert(std::is_integral_v<int_type>, "int_type must be an integer type");
    static_assert(fraction <= sizeof(int_type) * 8, "can not have more fractional bits than there are bits in the int");
    
    int_type v;
    
    constexpr fixed() = default;
    
    constexpr fixed(const fixed& other)=default;
    
    constexpr fixed& operator=(const fixed& other){
        v = other.v;
        return *this;
    }
    
    constexpr fixed(int_type i){
        v = i << frac_bits();
    }
    
    template<size_t S>
    constexpr fixed(fixed_construction_helper<S> helper){
    
        int64_t result = (helper.whole << fraction) | (helper.frac >> (64 - fraction));
        if((helper.frac >> (64-fraction-1)) & 1) result++;

        if(helper.negative) result = -result;
        v = result;
    }
    
    constexpr fixed& operator=(int i){
        v = i << frac_bits();
        return *this;
    }
    
    constexpr bool operator==(const fixed& other) const{
        return v == other.v;
    }
    
    template<typename S, size_t new_frac_bits>
    explicit constexpr operator fixed<S, new_frac_bits>() const{
        fixed<S, new_frac_bits> result;
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

template<typename T, size_t fraction>
inline constexpr fixed<T, fraction> fp_from_bits(T i){
    fixed<T, fraction> result;
    result.v = i;
    return result;
}

template<typename T, size_t fraction>
inline constexpr fixed<T, fraction> make_fixed(T i){
    fixed<T, fraction> result;
    result.v = i << fraction;
    return result;
}


template<typename T, size_t fraction, size_t S>
inline constexpr fixed<T, fraction> make_fixed(fixed_construction_helper<S> h){
    fixed<T, fraction> result(h);
    return result;
}

}//end namespace fixed_point
