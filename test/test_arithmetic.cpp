#include <array>

#include "test_helper.hpp"
#include "test_arithmetic.hpp"
#include "fixed_point_math.hpp"
#include "fixed_point_print.hpp"

using namespace fixed_point;

template<typename fp_t>
bool test_add_int_impl(int ai, int bi){
    constexpr size_t frac_bits = fp_t::frac_bits();
    fp_t a = ai;
    fp_t b = bi;
    fp_t c = a+b;
    bool passed = (c.to_int() == ai+bi);
    if(!passed){
        std::cout << "a: " << +a.to_int() << " ";
        std::cout << "b: " << +b.to_int() << " ";
        std::cout << "c: " << +c.v << " ";
        std::cout << "frac bits: " << frac_bits << " ai: " << ai << " bi: " << bi << " a+b: " << +c.to_int() << " should be: " << ai+bi << std::endl;
    }
    return passed;
}

template<typename fp_t>
bool test_sub_int_impl(int ai, int bi){
    constexpr size_t frac_bits = fp_t::frac_bits();
    fp_t a = ai;
    fp_t b = bi;
    fp_t c = a-b;
    bool passed = (c.to_int() == ai-bi);
    if(!passed){
        std::cout << "a: " << +a.to_int() << " ";
        std::cout << "b: " << +b.to_int() << " ";
        std::cout << "frac bits: " << frac_bits << " ai: " << ai << " bi: " << bi << " a-b: " << +c.to_int() << " should be: " << ai-bi << std::endl;
    }
    return passed;
}

template<typename fp_t>
bool test_mul_int_impl(int ai, int bi){
    constexpr size_t frac_bits = fp_t::frac_bits();
    fp_t a = ai;
    fp_t b = bi;
    fp_t c = a*b;
    bool passed = (c.to_int() == ai*bi);
    if(!passed){
        std::cout << "a: " << +a.to_int() << " ";
        std::cout << "b: " << +b.to_int() << " ";
        std::cout << "frac bits: " << frac_bits << " ai: " << ai << " bi: " << bi << " a*b: " << +c.to_int() << " should be: " << ai*bi << std::endl;
    }
    return passed;
}


bool test_int_arithmetic(){
    using fs_8_4 = fixed<int8_t, 4>;
    using fs_16_8 = fixed<int16_t, 8>;
    using fs_32_16 = fixed<int32_t, 16>;
    
    std::array arg1 = {1,2,3,7,14,23,255,1023,2016,-1,-2,-3,-7,-14,-23,-255,-1023,-2016};
    
    bool all_passed = true;
    bool passed = true;
    
    for(auto i : arg1){
        for(auto j : arg1){
            passed = true;
            if(std::abs(i + j) <= fs_8_4::max_as_int()) passed &= test_add_int_impl<fs_8_4>(i, j);
            if(std::abs(i + j) <= fs_16_8::max_as_int()) passed &= test_add_int_impl<fs_16_8>(i, j);
            if(std::abs(i + j) <= fs_32_16::max_as_int()) passed &= test_add_int_impl<fs_32_16>(i, j);
            if(!passed) log_msg("failed 'add int' test!");
            all_passed &= passed;
            
            passed = true;
            if(std::abs(i - j) <= fs_8_4::max_as_int()) passed &= test_sub_int_impl<fs_8_4>(i, j);
            if(std::abs(i - j) <= fs_16_8::max_as_int()) passed &= test_sub_int_impl<fs_16_8>(i, j);
            if(std::abs(i - j) <= fs_32_16::max_as_int()) passed &= test_sub_int_impl<fs_32_16>(i, j);
            if(!passed) log_msg("failed 'sub int' test!");
            all_passed &= passed;
            
            passed = true;
            if(std::abs(i * j) <= fs_8_4::max_as_int()) passed &= test_mul_int_impl<fs_8_4>(i, j);
            if(std::abs(i * j) <= fs_16_8::max_as_int()) passed &= test_mul_int_impl<fs_16_8>(i, j);
            if(std::abs(i * j) <= fs_32_16::max_as_int()) passed &= test_mul_int_impl<fs_32_16>(i, j);
            if(!passed) log_msg("failed 'sub int' test!");
            all_passed &= passed;
        }
    }
    
    return all_passed;
}


bool test_fractional_arithmetic(){
    using fs_32_16 = fixed<int32_t, 16>;
    using fs_32_24 = fixed<int32_t, 24>;
    
    bool all_passed = true;
    bool passed = true;
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = -2.173_fixp_t;
        auto c = a+b;
        passed &= (c == 0);
        if(!passed) log_msg("failed 'add fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        auto c = a + -2.173_fixp_t;
        passed &= (c == 0);
        if(!passed) log_msg("failed 'add fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        auto c = -2.173_fixp_t + a;
        passed &= (c == 0);
        if(!passed) log_msg("failed 'add fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 2.173_fixp_t;
        fs_32_16 c = 4.346_fixp_t;
        auto d = a+b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'add fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 2.173_fixp_t;
        fs_32_16 c = 0.0_fixp_t;
        auto d = a-b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'sub fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 7.183_fixp_t;
        fs_32_16 c = -5.01_fixp_t;
        auto d = a-b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'sub fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = -2.173_fixp_t;
        fs_32_24 c = -4.721929_fixp_t;
        auto d = a*b;
        passed &= (std::abs(c.v - d.v) <= 1);
        log_msg(std::to_string(d.v));
        log_msg(std::to_string(c.v));
        if(!passed) log_msg("failed 'mul fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = 2.173_fixp_t;
        fs_32_24 c = 1.0_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = 7.183_fixp_t;
        fs_32_24 c = 0.30251983850758735_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = -7.183_fixp_t;
        fs_32_24 c = -0.30251983850758735_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 12.8162_fixp_t;
        fs_32_24 b = 3.579972066930132_fixp_t;
        fs_32_24 c = approx_sqrt(a);
        passed &= same_top_most_bit(b,c);
        if(!passed) log_msg("failed 'approx sqrt fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 0.008162_fixp_t;
        fs_32_24 b = 0.09034378783292187_fixp_t;
        fs_32_24 c = approx_sqrt(a);
        passed &= same_top_most_bit(b,c);
        if(!passed) log_msg("failed 'approx sqrt fractional' test!");
        all_passed &= passed;
    }
    
        {
        passed = true;
        fs_32_24 a = 12.8162_fixp_t;
        fs_32_24 b = 3.579972066930132_fixp_t;
        fs_32_24 c = approx_sqrt(a);
        fs_32_24 d = better_approx_sqrt(a);
        auto error_approx = b-c;
        auto error_better = b-d;
        passed &= (std::abs(error_approx.v) >= std::abs(error_better.v));
        if(!passed) log_msg("failed 'approx sqrt fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 0.008162_fixp_t;
        fs_32_24 b = 0.09034378783292187_fixp_t;
        fs_32_24 c = approx_sqrt(a);
        fs_32_24 d = better_approx_sqrt(a);
        auto error_approx = b-c;
        auto error_better = b-d;
        passed &= (std::abs(error_approx.v) >= std::abs(error_better.v));
        if(!passed) log_msg("failed 'approx sqrt fractional' test!");
        all_passed &= passed;
    }
    
    
    {
        passed = true;
        fs_32_24 a = 12.8162_fixp_t;
        fs_32_24 b = 3.579972066930132_fixp_t;
        fs_32_24 c = sqrt(a);
        passed &= same_up_to_n_bits(b,c,2);
        if(!passed) log_msg("failed 'sqrt fractional' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 0.008162_fixp_t;
        fs_32_24 b = 0.09034378783292187_fixp_t;
        fs_32_24 c = sqrt(a);
        passed &= same_up_to_n_bits(b,c,2);
        if(!passed) log_msg("failed 'sqrt fractional' test!");
        all_passed &= passed;
    }
    
    
    
    return all_passed;
}

bool test_arithmetic_comparisons(){
    using fixp = fixed<int32_t, 16>;
    
    bool all_passed = true;
    bool passed = true;
    
    {
        passed = true;
        fixp a = 1.234_fixp_t;
        fixp b = 1.234_fixp_t;
        
        passed &=  (a==b);
        passed &= !(a!=b);
        passed &=  (a<=b);
        passed &=  (a>=b);
        passed &= !(a<b);
        passed &= !(a>b);
        if(!passed) log_msg("failed 'arithmetic comparisons' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 5.234_fixp_t;
        fixp b = 1.234_fixp_t;
        
        passed &= !(a==b);
        passed &=  (a!=b);
        passed &= !(a<=b);
        passed &=  (a>=b);
        passed &= !(a<b);
        passed &=  (a>b);
        if(!passed) log_msg("failed 'arithmetic comparisons' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 67.162_fixp_t;
        fixp b = 99.112_fixp_t;
        
        passed &= !(a==b);
        passed &=  (a!=b);
        passed &=  (a<=b);
        passed &= !(a>=b);
        passed &=  (a<b);
        passed &= !(a>b);
        if(!passed) log_msg("failed 'arithmetic comparisons' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 67.162_fixp_t;
        
        passed &= (a==67.162_fixp_t);
        passed &= (a!=24.22_fixp_t);
        passed &= (a<=172.95_fixp_t);
        passed &= (a>=15.123_fixp_t);
        passed &= (a<1123.723_fixp_t);
        passed &= (a>17.1_fixp_t);
        if(!passed) log_msg("failed 'arithmetic comparisons' test!");
        all_passed &= passed;
    }
    
    
    return all_passed;
}

bool test_arithmetic_utility_functions(){
    using fixp = fixed<int32_t, 16>;
    
    bool all_passed = true;
    bool passed = true;
    
    {
        passed = true;
        fixp a = 67.162_fixp_t;
        passed &= abs(a) == a;
        if(!passed) log_msg("failed 'abs' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = -67.162_fixp_t;
        passed &= abs(a) == -a;
        if(!passed) log_msg("failed 'abs' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 14.916_fixp_t;
        fixp b = 12.763_fixp_t;
        fixp c = 67.162_fixp_t;
        passed &= clamp(a,b,c) == a;
        if(!passed) log_msg("failed 'clamp' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 11.916_fixp_t;
        fixp b = 12.763_fixp_t;
        fixp c = 67.162_fixp_t;
        passed &= clamp(a,b,c) == b;
        if(!passed) log_msg("failed 'clamp' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fixp a = 74.916_fixp_t;
        fixp b = 12.763_fixp_t;
        fixp c = 67.162_fixp_t;
        passed &= clamp(a,b,c) == c;
        if(!passed) log_msg("failed 'clamp' test!");
        all_passed &= passed;
    }
    
    return all_passed;
}

bool test_arithmetic(){
    bool all_passed = true;
    
    all_passed &= test_int_arithmetic();
    all_passed &= test_fractional_arithmetic();
    all_passed &= test_arithmetic_comparisons();
    all_passed &= test_arithmetic_utility_functions();
    
    return all_passed;
}









