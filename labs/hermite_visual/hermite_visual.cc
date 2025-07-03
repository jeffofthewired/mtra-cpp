/*
This app will output a bunch of data points into a file
i will then visualise this in R or something
*/

#include <iostream>
#include <vector>

#include "mtra/dsp/hermite_interpolation.h"
#include "mtra/span/span.h"

constexpr static std::size_t POLY_NUM = 32;
constexpr static std::size_t MAX_LEN = 1<<16;
constexpr static std::size_t BUF_SIZE = 256;
constexpr static std::size_t WIN_SIZE = 256;

template <typename T>
struct file_output {
    auto write(float val) -> void {
        std::cout << val << "\n";
    }
};

int main() {
    std::cout << "hello world!\n";
    file_output<float> os;
    mtra::hermite_interpolator<file_output<float>, float, float> interp{os, 0., 0.1, 0., 1.};
    std::vector<float> v(2048, 0.);
    mtra::span<float> sp(v.data(), v.size());
    interp.write(sp);
}
