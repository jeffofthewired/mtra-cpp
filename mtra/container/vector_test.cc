#include <gtest/gtest.h>

#include "vector.h"

TEST(IntVector, BasicOps) {
    mtra::vector<int> v;
    EXPECT_EQ(v.size(), 0u);

    constexpr std::size_t test_size = 300;
    for (std::size_t i = 0; i < test_size; ++i) {
        v.push_back(i);
    }
    for (std::size_t i = 0; i < test_size; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(IntVector, IteratorOps) {
    mtra::vector<int> v;
    constexpr std::size_t test_size = 300;
    for (std::size_t i = 0; i < test_size; ++i) {
        v.push_back(i);
    }

    // forward iterator test
    mtra::vector<int>::iterator iter = v.begin();
    for (std::size_t i = 0; i < test_size; ++i, ++iter) {
        EXPECT_EQ(*iter, i);
    }

    // reverse iterator test
    // TODO: change begin to rbegin
    mtra::reverse_iterator<mtra::vector<int>::iterator> rev_iter = v.rbegin();
    for (std::size_t i = 0; i < test_size; ++i, ++rev_iter) {
        EXPECT_EQ(*rev_iter, test_size - 1 - i);
    }
}
 
// Okay now do some tests involving iterators

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
