#include <array>

#include "test_helper.hpp"
#include "test_arithmetic.hpp"
#include "fixed_point_math.hpp"



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
    using fs_8_4 = fixed_point_t<int8_t, 4>;
    using fs_16_8 = fixed_point_t<int16_t, 8>;
    using fs_32_16 = fixed_point_t<int32_t, 16>;
    
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


bool test_float_arithmetic(){
    using fs_32_16 = fixed_point_t<int32_t, 16>;
    using fs_32_24 = fixed_point_t<int32_t, 24>;
    
    bool all_passed = true;
    bool passed = true;
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = -2.173_fixp_t;
        auto c = a+b;
        passed &= (c == 0);
        if(!passed) log_msg("failed 'add float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 2.173_fixp_t;
        fs_32_16 c = 4.346_fixp_t;
        auto d = a+b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'add float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 2.173_fixp_t;
        fs_32_16 c = 0.0_fixp_t;
        auto d = a-b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'sub float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_16 a = 2.173_fixp_t;
        fs_32_16 b = 7.183_fixp_t;
        fs_32_16 c = -5.01_fixp_t;
        auto d = a-b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'sub float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = -2.173_fixp_t;
        fs_32_24 c = -4.721929_fixp_t;
        auto d = a*b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'mul float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = 2.173_fixp_t;
        fs_32_24 c = 1.0_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = 7.183_fixp_t;
        fs_32_24 c = 0.30251983850758735_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div float' test!");
        all_passed &= passed;
    }
    
    {
        passed = true;
        fs_32_24 a = 2.173_fixp_t;
        fs_32_24 b = -7.183_fixp_t;
        fs_32_24 c = -0.30251983850758735_fixp_t;
        auto d = a/b;
        passed &= (std::abs(c.v - d.v) <= 1);
        if(!passed) log_msg("failed 'div float' test!");
        all_passed &= passed;
    }
    
    return all_passed;
}

bool test_arithmetic_comparisons(){
    using fixp = fixed_point_t<int32_t, 16>;
    
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
    
    
    return all_passed;
}




bool test_arithmetic(){
    bool all_passed = true;
    
    all_passed &= test_int_arithmetic();
    all_passed &= test_float_arithmetic();
    all_passed &= test_arithmetic_comparisons();
    
    return all_passed;
}









