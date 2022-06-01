#include <cstdint>

#include "test_ctor.hpp"
#include "test_helper.hpp"
#include "fixed_point_math.hpp"
#include "fixed_point_float_conversions.hpp"

using namespace fixed_point;

bool test_basic_constructor(){
    fixed<int8_t, 4> x = 5;
    return x.v == 5 << 4;
} 

bool test_int_constructor(){
    auto x = fp_from_bits<int8_t, 4>(7 << 4);
    return x.v == (7<<4);
}

bool test_float_constructor(){
    auto x = fp_from_float<int8_t, 4>(2.5f);
    return x.v == 0b101000;
}

bool test_copy_constructor(){
    fixed<int8_t, 4> x = 7;
    auto y = x;
    return x.v == y.v;
}

bool test_assignment_operator(){
    fixed<int8_t, 4> x = 7;
    fixed<int8_t, 4> y;
    y = x;
    return x.v == y.v;
}

bool test_float_constant_constructor(){
    fixed<int32_t, 16> x = 43.167_fixp_t;
    auto y = fp_from_float<int32_t, 16>(43.167f);
    return x.v == y.v;
}

bool test_ctor(){
    bool all_passed = true;
    bool passed = true;
    
    passed = test_basic_constructor();
    if(!passed) log_msg("failed basic constructor test!");
    all_passed &= passed;
    
    passed = test_int_constructor();
    if(!passed) log_msg("failed int constructor test!");
    all_passed &= passed;
    
    passed = test_float_constructor();
    if(!passed) log_msg("failed float constructor test!");
    all_passed &= passed;
    
    passed = test_copy_constructor();
    if(!passed) log_msg("failed copy constructor test!");
    all_passed &= passed;
    
    passed = test_assignment_operator();
    if(!passed) log_msg("failed assignment operator test!");
    all_passed &= passed;
    
    passed = test_float_constant_constructor();
    if(!passed) log_msg("failed float constant constructor test!");
    all_passed &= passed;
    
    return all_passed;
}
