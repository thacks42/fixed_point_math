#pragma once

#include <cstdint>
#include <array>
#include <type_traits>
#include <string_view>
#include <cassert>
#include <compare>

namespace fixed_point{

struct fixed_construction_helper{
    std::string_view sv;
    bool negative = false;
};


constexpr inline fixed_construction_helper operator "" _fixp_t(const char* str){
    fixed_construction_helper helper{str};
    return helper;
}

constexpr inline fixed_construction_helper operator-(fixed_construction_helper helper){
    helper.negative = !helper.negative;
    return helper;
}

constexpr inline fixed_construction_helper operator+(fixed_construction_helper helper){
    return helper;
}


template<typename T, size_t fraction>
struct fixed{
    using int_type = T;
    static_assert(std::is_integral_v<int_type>, "int_type must be an integer type");
    static_assert(fraction <= sizeof(int_type) * 8, "can not have more fractional bits than there are bits in the int");
    
    int_type v;
    
    constexpr fixed(){
        v = 0;
    }
    
    constexpr fixed(const fixed& other){
        v = other.v;
    }
    
    constexpr fixed& operator=(const fixed& other){
        v = other.v;
        return *this;
    }
    
    constexpr fixed(int_type i){
        v = i << frac_bits();
    }
    
    constexpr fixed(fixed_construction_helper helper){
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
constexpr inline fixed<T, fraction> operator+(const fixed<T, fraction> a, const fixed<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v + b.v);
}


template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(const fixed<T, fraction> a, const fixed<T, fraction> b){
    return fp_from_bits<T, fraction>(a.v - b.v);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator-(const fixed<T, fraction> a){
    return fp_from_bits<T, fraction>(-a.v);
}

template<typename T, size_t fraction>
constexpr inline fixed<T, fraction> operator*(const fixed<T, fraction> a, const fixed<T, fraction> b){
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
constexpr inline fixed<T, fraction> operator/(const fixed<T, fraction> a, const fixed<T, fraction> b){
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
        static_assert(!std::is_same_v<T, T>, "multiplication of 64 bit fixed point numbers is not implemented, sorry");
    }
}

template<typename T, size_t fraction>
inline constexpr std::strong_ordering operator<=>(fixed<T, fraction> a, fixed<T, fraction> b){
    return a.v <=> b.v;
}



}//end namespace fixed_point












