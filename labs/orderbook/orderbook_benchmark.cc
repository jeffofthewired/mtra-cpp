#include "orderbook.h"

#define ANNA 4774
#define BOB 808
#define ZOE 203

auto order(price_t price, quant_t quantity, tid_t trader_id) -> Order {
    Order order;
    order.price = price;
    order.quantity = quantity;
    order.trader_id = trader_id;
    return order;
}

int main() {
    std::cout << "hello world!" << std::endl;
    MatchingEngine m;

    m.insert_bid(order(1000, 10, ANNA));
    m.insert_bid(order(1001, 10, BOB));
    m.insert_ask(order(999, 200, ZOE));
    m.insert_bid(order(1000, 100, ANNA));
    m.insert_bid(order(1000, 100, BOB));

    std::cout << "end of simulation" << std::endl;
}
