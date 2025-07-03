/*
This app will output a bunch of data points into a file
i will then visualise this in R or something
*/

#include <iostream>
#include <vector>

#include "mtra/dsp/hermite_interpolation.h"
#include "mtra/span/span.h"

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
    std::vector<float> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    mtra::span<float> sp(v.data(), v.size());
    interp.write(sp);
}
