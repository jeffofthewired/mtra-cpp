#pragma once

#include <cstring>
#include <immintrin.h>

// only works on some compilers
#define force_inline __attribute__((always_inline)) inline

namespace mtra {

// ensure that k_floats_per_simd matches the type used (__m256)
// i will use __m256 (AVX instruction set) 
// just allows for vectorized operations
template <std::size_t N>
class poly_float {
// aliases
private:

    using size_type = std::size_t;

    // number of floats in an AVX2 float (__m256)
    static constexpr size_type k_floats_per_simd = 8;
    static constexpr size_type k_simd_per_poly_float = (N + 7)/8;
    static constexpr size_type k_floats_per_poly_float = k_simd_per_poly_float * k_floats_per_simd;

    union _pun_simd {
        // check constants.h to see how these are defined
        float scalar[k_floats_per_poly_float];
        __m256 vector[k_simd_per_poly_float];
    };

// public member variables
public:
    _pun_simd data_;

// static functions
public:
    force_inline static poly_float zero() {
        poly_float x;
        std::memset(x.data_.scalar, 0, k_floats_per_poly_float * sizeof(float));
        return x;
    }

    force_inline static poly_float broadcast(float x) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_set1_ps(x);
        }
        return result;
    }

// operator overloads
public:
    force_inline float& operator[](size_type index) {
        return data_.scalar[index];
    } 

    force_inline friend poly_float operator+(const poly_float& lhs, const poly_float& rhs) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_add_ps(lhs.data_.vector[i], rhs.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float operator-(const poly_float& lhs, const poly_float& rhs) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_sub_ps(lhs.data_.vector[i], rhs.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float operator*(const poly_float& lhs, const poly_float& rhs) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_mul_ps(lhs.data_.vector[i], rhs.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float operator/(const poly_float& lhs, const poly_float& rhs) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_div_ps(lhs.data_.vector[i], rhs.data_.vector[i]);
        }
        return result;
    }
    
    // binary arithmetic overloads between a scalar (float) and apoly_floatt
    // just broadcast the float to a vector type using set1_ps
    force_inline friend poly_float operator*(float lhs, const poly_float& rhs) {
        auto lhs_repeated = _mm256_set1_ps(lhs);
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_mul_ps(lhs_repeated, rhs.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float operator+(float lhs, const poly_float& rhs) {
        auto lhs_repeated = _mm256_set1_ps(lhs);
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_add_ps(lhs_repeated, rhs.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float operator/(const poly_float& lhs, float rhs) {
        auto rhs_repeated = _mm256_set1_ps(rhs);
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_div_ps(lhs.data_.vector[i], rhs_repeated);
        }
        return result;
    }

    force_inline friend poly_float operator%(const poly_float& lhs, const poly_float& rhs) {
        poly_float quotient = lhs / rhs;
        poly_float quotient_floor = floor(quotient);
        poly_float remainder = lhs - (rhs * quotient_floor);
        return remainder;
    }

    force_inline friend poly_float operator%(const poly_float& lhs, float rhs) {
        poly_float quotient = lhs / rhs;
        poly_float quotient_floor = floor(quotient);
        poly_float remainder = lhs - (rhs * quotient_floor);
        return remainder;
    }

    force_inline friend poly_float floor(const poly_float& x) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_floor_ps(x.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float max(const poly_float& a, const poly_float& b) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_max_ps(a.data_.vector[i], b.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float min(const poly_float& a, const poly_float& b) {
        poly_float result;
        for (size_type i = 0; i < k_simd_per_poly_float; ++i) {
            result.data_.vector[i] = _mm256_min_ps(a.data_.vector[i], b.data_.vector[i]);
        }
        return result;
    }

    force_inline friend poly_float clamp(const poly_float& x, float clamp_min, float clamp_max) {
        return max(poly_float::broadcast(clamp_min), min(poly_float::broadcast(clamp_max), x));
    }
};

}
