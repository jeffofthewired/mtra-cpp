#pragma once

#include <span>

#include "mtra/simd/poly_float.h"

namespace mtra {

// think about how to generalize this / convert to templates

using default_type = poly_float<32>;
//using default_type = float;

// for stream processing
// set some destination output
// input samples one by one, and it will automatically output
// inputing one by one vs as batches??
template <typename OStream, typename sample_type = default_type, typename offset_type = default_type>
class hermite_interpolator {
// special member functions
public: // use this to get the sample rate

private:
    offset_type current_;
    offset_type interval_;

public:
    auto write(std::span<sample_type> s) -> void {

    }

    auto write(sample_type s) -> void {

    }
};

// TODO: move into a .cc file, or move into the class
template <typename sample_type = default_type, typename offset_type = default_type>
auto hermite_interpolate(
    sample_type s0, 
    sample_type s1, 
    sample_type s2, 
    sample_type s3, 
    offset_type offset
) -> sample_type {
    sample_type slope0 = 0.5f * (s2 - s0);
    sample_type slope1 = 0.5f * (s3 - s1);
    sample_type v = s1 - s2;
    sample_type w = slope0 + v;
    sample_type a = w + v + slope1;
    sample_type b_neg = w + a;
    // check if multiplying sample_types and offset_types is even possible
    sample_type stage1 = a * offset - b_neg;
    sample_type stage2 = stage1 * offset + slope0;
    sample_type result = stage2 * offset + s1;
    return result;
}

}
