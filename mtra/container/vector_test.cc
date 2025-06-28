#include <gtest/gtest.h>
#include "vector.h"

TEST(VectorTest, BasicOperations) {
    mtra::vector<int> v;
    EXPECT_EQ(v.size(), 0u);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
