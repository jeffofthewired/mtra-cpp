#include <iostream>
#include <thread>
#include "mpmc_stack_1.h"

struct baby {
};

struct widget {
    int value_;

public:
    widget(int value) : value_{value} {}
    
    widget(const widget& other) : value_{other.value_} {
        if (value_ == 5) {
            //throw baby();
        }
    }
};

int main() {
    std::cout << "Welcome!\n";
    std::cout << "Creating stack!\n";
    mtra::lockfree::mpmc_stack_1<widget> s{};

    auto p_func = [&](int offset){
        try {
            for (int i = offset; i < offset + 1000; ++i) {
                s.push(widget(i));
            }
            for (int i = 0; i < 1000; ++i) {
                std::cout << s.pop().value_ << std::endl;
            }
        } catch (baby b) {
            std::cout << "caught baby!\n";
        }
    };

    std::thread t1(p_func, 60000);
    std::thread t2(p_func, 70000);
    t1.join();
    t2.join();

    std::cout << "thread joined\n";
}
