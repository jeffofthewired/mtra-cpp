#pragma once

#include <cstddef>
#include <set>
#include <unordered_map>

// __id_t is reused wtf
using price_t = std::size_t; // price
using quant_t = std::size_t; // quantity
using id_type = std::size_t; // id
using ts_t = std::size_t; // timestamp

// should price be stored in here?
struct Order {
    price_t price;
    quant_t quantity;
    ts_t timestamp;
    id_type order_id;
    id_type trader_id;
};

// higher priority at the beginning
struct Bid {
    auto operator()(const Order& a, const Order& b) -> bool {
        if (a.price == b.price) {
            return a.timestamp < b.timestamp;
        } else {
            return a.price > b.price;
        }
    }
};

struct Ask {
    auto operator()(const Order& a, const Order& b) -> bool {
        if (a.price == b.price) {
            return a.timestamp < b.timestamp;
        } else {
            return a.price < b.price;
        }
    }
};

// basic price time priority matching engine
// hashmap containers pointers to rbt nodes
template <typename Side>
class NaiveOrderbook {
private:
    using pr_lookup_t = std::multiset<Order, Side>;
    using id_lookup_t = std::unordered_map<id_type, typename pr_lookup_t::iterator>;

private:
    id_type seq_no_ = 0;
    pr_lookup_t price_lookup_;
    id_lookup_t id_lookup_;

public:
    NaiveOrderbook() : price_lookup_(Side()) {}

    auto insert(Order order) -> id_type {
        order.timestamp = seq_no_;
        order.order_id = ++seq_no_;
        auto order_iter = price_lookup_.insert(order);
        id_lookup_[order.order_id] = order_iter;
        return order.order_id;
    }

    // returns updated order to insert into the other side
    auto match(Order order) -> Order {
        while (true) {
            auto best = price_lookup_.lower_bound({order.price, 0, -1, 0, 0});
        }
    }

    auto remove(id_type order_id) -> void {
        auto removed_node = id_lookup_[order_id];
        price_lookup_.erase(removed_node);
        id_lookup_.erase(order_id);
    }
};

