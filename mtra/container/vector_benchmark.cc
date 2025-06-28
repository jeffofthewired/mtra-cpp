#include <vector>

#include <benchmark/benchmark.h>
#include "vector.h"

static void BM_push_back_mtra(benchmark::State& state) {
    for (auto _ : state) {
        mtra::vector<int> v;
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_push_back_mtra)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_push_back_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v;
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_push_back_std)->Arg(100)->Arg(1000)->Arg(10000);

BENCHMARK_MAIN();

