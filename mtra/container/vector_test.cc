#include <gtest/gtest.h>

#include "vector.h"

// Tests

TEST(VectorTest, BasicOperations) {
    mtra::vector<int> v;
    EXPECT_EQ(v.size(), 0u);
}

// Okay now do some tests involving iterators

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
