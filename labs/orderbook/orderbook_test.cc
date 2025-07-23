#include <gtest/gtest.h>

#include "orderbook.h"

/*
struct Order {
    price_t price;
    quant_t quantity;
    ts_t timestamp;
    id_type order_id;
    id_type trader_id;
};
*/

TEST(NaiveOrderbook, BasicOps) {
    MatchingEngine m;
    m.insert_bid({});
}

// Okay now do some tests involving iterators

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

