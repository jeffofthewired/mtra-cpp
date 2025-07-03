#pragma once

#include <cstddef>

#include "mtra/span/span.h"
#include "mtra/simd/poly_float.h"

namespace mtra {

// think about how to generalize this / convert to templates
    using default_type = poly_float<32>;
//  using default_type = float;

template <typename T>
class _hermite_span_pair {
// aliases
public:
    using size_type = std::size_t;

private:
    span<T> span1_;
    span<T> span2_;
    size_type size1_;
    size_type size2_;
    size_type size_;

public:
    _hermite_span_pair(span<T> span1, span<T> span2)
    : span1_{span1}, span2_{span2}, size1_{span1.size()}, size2_{span2.size()}, size_{size1_ + size2_}
    {}

    auto operator[](size_type i) -> T& {
        // this likelihood is here specically so it works well
        // with hermite interpolation
        if (i < size1_) [[unlikely]] {
            return span1_[i];
        } else {
            return span2_[i - size1_];
        }
    }

    auto size() const -> size_type {
        return size_;
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

// for stream processing simd samples
// set some destination output
// input samples one by one, and it will automatically output
// inputing one by one vs as batches??
// offset_type must have certain operators including <
template <typename ostream_type, typename sample_type = default_type, typename offset_type = default_type>
class hermite_interpolator {
// aliases
    using size_type = std::size_t;

private:
    ostream_type ostream_;
    // 
    offset_type curr_output_pos_;
    // the interval at which
    offset_type output_pos_incr_;
    // the current sample number (ideally an integer)
    offset_type curr_input_pos_;
    // how much current num will be incremented every sample (ideally 1)
    offset_type input_pos_incr_;

    // historical samples
    sample_type _history_buf_[3];
    span<sample_type> history_;
    bool empty_history_ = true;

// special member functions
public:
    // the reason why all this redundant information is needed is so that
    // this class works with both regular floats and SIMD floats
    hermite_interpolator(
        ostream_type ostream, 
        offset_type init_output_pos, // must be a broadcasted number
        offset_type output_pos_incr, // must be a broadcasted number
        offset_type init_input_pos, // must be 0 or broadcasted 0
        offset_type input_pos_incr // must be 1 or broadcasted 1
    )
    :   ostream_{ostream},
        curr_output_pos_{init_output_pos},
        output_pos_incr_{output_pos_incr},
        curr_input_pos_{init_input_pos},
        input_pos_incr_{input_pos_incr},
        history_{_history_buf_, 3}
    {}

// regular member functions
public:

    auto write(span<sample_type> sample_span) -> void {
        // populate history
        if (empty_history_) [[unlikely]] {
            history_[0] = sample_span[0];
            history_[1] = sample_span[0];
            history_[2] = sample_span[0];
        }
        // concatenate the last 3 historical samples, with the new samples
        _hermite_span_pair<sample_type> full_span{history_, sample_span};

        // iterate through carefully
        typename _hermite_span_pair<sample_type>::size_type it = 0;
        for (; it + 3 < full_span.size(); ++it) {
            while (curr_input_pos_ - input_pos_incr_ > curr_output_pos_) {
                // there is a meaning to this
                sample_type interpolated_val = hermite_interpolate(
                    full_span[it], full_span[it + 1], full_span[it + 2], full_span[it + 3],
                    curr_output_pos_ - curr_input_pos_ + input_pos_incr_ + input_pos_incr_
                );
                ostream_.write(interpolated_val);
                curr_output_pos_ += output_pos_incr_;
            }
            curr_input_pos_ += input_pos_incr_;
        }

        // update the history
        history_[0] = full_span[it];
        history_[1] = full_span[it + 1];
        history_[2] = full_span[it + 2];
    }

    // TODO: implement this function later, but should be easier
    auto write(sample_type s) -> void {}
};


}
