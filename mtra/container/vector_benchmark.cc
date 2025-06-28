#include <benchmark/benchmark.h>
#include "vector.h"

static void BM_push_back(benchmark::State& state) {
    for (auto _ : state) {
        mtra::vector<int> v;
        for (int i = 0; i < state.range(0); ++i) {
            v.push_back(i);
        }
    }
}
BENCHMARK(BM_push_back)->Arg(100)->Arg(1000)->Arg(10000);

BENCHMARK_MAIN();
