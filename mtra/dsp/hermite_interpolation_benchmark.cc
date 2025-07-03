#include <vector>
#include <cstddef>

#include <benchmark/benchmark.h>

#include "mtra/simd/poly_float.h"
#include "mtra/dsp/hermite_interpolation.h"
#include "mtra/span/span.h"

//////// SETUP ////////////////////////////////////////////////////////////////////

constexpr static std::size_t POLY_NUM = 32;
constexpr static std::size_t MAX_LEN = 1<<16;
constexpr static std::size_t BUF_SIZE = 256;
constexpr static std::size_t WIN_SIZE = 256;
using poly = mtra::poly_float<POLY_NUM>;

// ostream type
template <typename T>
struct buffer_output {
    T* buf_;
    std::size_t cur_ = 0;
    std::size_t size_;
    buffer_output(std::size_t size) : buf_{new T[size]}, size_{size} {}
    auto write(T val) -> void {
        buf_[cur_] = val;
        cur_ = (cur_ + 1) % size_;
    }
};

 
//////// BENCHMARKS //////////////////////////////////////////////////////////////

static void BM_float_interpolation(benchmark::State& state) {
    std::vector<std::vector<float>> inputs(POLY_NUM, std::vector<float>(MAX_LEN, 0));
    std::vector<buffer_output<float>> ostreams(POLY_NUM, buffer_output<float>(BUF_SIZE));
    std::vector<mtra::hermite_interpolator<buffer_output<float>, float, float>> interpolators;
    for (auto os : ostreams) {
        interpolators.emplace_back(os, 0., 0.1, 0., 1.);
    }

    for (auto _ : state) {
        auto len = state.range(0);

        for (std::size_t i = 0; i < len; i += WIN_SIZE) {
            // iterates up within the window
            for (std::size_t v = 0; v < POLY_NUM; ++v) {
                mtra::span<float> window{inputs[v].data() + i, WIN_SIZE};
                interpolators[v].write(window);
            }
        }
    }
}
BENCHMARK(BM_float_interpolation)->Arg(1<<10)->Arg(1<<15);

static void BM_polyfloat_interpolation(benchmark::State& state) {
    auto len = state.range(0);
    std::vector<std::vector<float>> inputs(POLY_NUM, std::vector<float>(len, 0));
    buffer_output<poly> os{BUF_SIZE};
    mtra::hermite_interpolator<buffer_output<poly>, poly, poly> interpolator{
        os, poly::broadcast(0), poly::broadcast(0.1), poly::broadcast(0), poly::broadcast(1)
    };
    // the thing to store the current span in
    std::vector<poly> collected(WIN_SIZE);

    for (auto _ : state) {
        auto len = state.range(0);
        for (std::size_t i = 0; i < len; i += WIN_SIZE) {
            // first load into these polyfloats
            for (std::size_t sample = 0; sample < WIN_SIZE; ++sample) {

                for (std::size_t v = 0; v < POLY_NUM; ++v) {
                    collected[0].data_.scalar[v] = inputs[v][i + sample];
                }
            }

            mtra::span<poly> window{collected.data(), WIN_SIZE};
            interpolator.write(window);
        }
    }
}
BENCHMARK(BM_polyfloat_interpolation)->Arg(1<<10)->Arg(1<<15);

BENCHMARK_MAIN();

