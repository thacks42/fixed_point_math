# fixed point math library

this repository contains a templated, header-only fixed point math library.

# Features

supported features so far:

* arbitrary integer types and number of fractional bits
* all basic arithmetic operations plus square root
* boolean comparisons
* construction from user defined literals
* conversion from/to strings
* utility functions to access the internal bit representation
* compiles to "the correct assembly" on both gcc and clang using `O1` and up.


# Example usage

```cpp
using namespace fixed_point;
using fp32_16 = fixed<int32_t, 16>;
fp32_16 a = 2.173_fixp_t;
fp32_16 b = 5.183_fixp_t;
auto c = a + b;
auto d = make_fixed<int32_t, 16>(2);
auto e = c * d - a;
debug_print(e); //prints: "binary:            1100.1000100111111011 as double:      12.538986206055"
```

# Headers

`fixed_point_type.hpp`:

contains the basic type definitions as well as anything related to construction and basic conversions

`fixed_point_math.hpp`:

contains the arithmetic operations

`fixed_point_print.hpp`:

contains helpers to print fixed point values. Makes use of `double`s as well as including `fmt/core.h` and `iostream`.
Do **not** include this on a target that doesn't have these libraries or floating point support! This header is meant for debugging purposes.

`fixed_point_float_conversions.hpp`:

contains functions to convert from/to floats. Do not include this if you don't have floating point support on your target!

