#pragma once

#include <cstddef>
#include <set>
#include <unordered_map>
#include <optional>
#include <iostream>

using price_t = std::size_t; // price
using quant_t = std::size_t; // quantity
using oid_t = std::size_t; // order id
using tid_t = std::size_t; // trader id
using ts_t = std::size_t; // timestamp

struct Order {
    price_t price;
    mutable quant_t quantity; // doesn't affect ordering
    tid_t trader_id;
    ts_t timestamp;
    oid_t order_id;
};

// populating these fields might require some if constexpr
// do we care which one was the aggressor??
struct Trade {
    price_t price;
    quant_t quantity;
    tid_t buyer_id;
    tid_t seller_id;
    tid_t aggressor_id;
};

// comparators for what makes a "more competitive" price
// more competitive = larger
// TODO: the fact that set_trade_sides exists is pretty weird
// theres side-based logic within each of these structs
struct BidPolicy {
    struct cmp {
        auto operator()(const Order& a, const Order& b) const -> bool {
            if (a.price == b.price) {
                return a.timestamp < b.timestamp;
            } else {
                return a.price > b.price;
            }
        }
    };

    auto set_trade_sides(Trade& trade, Order resting, Order aggressing) -> void {
        trade.buyer_id = resting.trader_id;
        trade.seller_id = aggressing.trader_id;
        trade.aggressor_id = aggressing.trader_id;
    }
};

struct AskPolicy {
    struct cmp {
        auto operator()(const Order& a, const Order& b) const -> bool {
            if (a.price == b.price) {
                return a.timestamp < b.timestamp;
            } else {
                return a.price < b.price;
            }
        }
    };

    auto set_trade_sides(Trade& trade, Order resting, Order aggressing) -> void {
        trade.seller_id = resting.trader_id;
        trade.buyer_id = aggressing.trader_id;
        trade.aggressor_id = aggressing.trader_id;
    }
};

// basic price time priority matching engine
// hashmap containers pointers to rbt nodes
// an orderbook that stores "more competitive orders" near the top
template <typename SidePolicy>
class NaiveOrderbookSide {
private:
    using pr_lookup_t = std::multiset<Order, typename SidePolicy::cmp>;
    using id_lookup_t = std::unordered_map<oid_t, typename pr_lookup_t::iterator>;

private:
    oid_t seq_no_ = 0;
    SidePolicy side_policy_;
    typename SidePolicy::cmp side_cmp_;
    pr_lookup_t price_lookup_;
    id_lookup_t id_lookup_;

public:
    NaiveOrderbookSide() : side_policy_{}, side_cmp_{}, price_lookup_(side_cmp_) {}

    auto insert(Order order) -> oid_t {
        order.timestamp = seq_no_;
        order.order_id = ++seq_no_;
        auto order_iter = price_lookup_.insert(order);
        id_lookup_[order.order_id] = order_iter;
        return order.order_id;
    }

    // returns remaining order
    auto match(Order order) -> std::optional<Order> {
        std::cout << "\tattempting match...\n";
        // match the agressing order with anything in the orderbook side that has a better price
        while (true) {
            // check if the aggressing order overlaps with the best price
            auto best = price_lookup_.rbegin();
            if (best == price_lookup_.rend() || side_cmp_(order, *best)) {
                std::cout << "\tterminating match (no better prices)...\n";
                break;
            }
            if (order.quantity <= best->quantity) {
                Trade trade;
                trade.price = best->price;
                trade.quantity = order.quantity;
                side_policy_.set_trade_sides(trade, *best, order);
                execute(trade);

                best->quantity -= order.quantity;
                if (best->quantity == 0) {
                    remove(best->order_id);
                }
                std::cout << "\tterminating match (aggressor exhausted)...\n";
                return std::optional<Order>();
            } else {
                Trade trade;
                trade.price = best->price;
                trade.quantity = best->quantity;
                side_policy_.set_trade_sides(trade, *best, order);
                execute(trade);

                order.quantity -= best->quantity;
                remove(best->order_id);
            }
        }

        return std::optional<Order>(order);
    }

    auto remove(oid_t order_id) -> void {
        auto removed_node = id_lookup_[order_id];
        price_lookup_.erase(removed_node);
        id_lookup_.erase(order_id);
    }

private:
    // logic to run whenever a trade gets executed
    // right now it just does nothing
    // in practice it should notify the traders somehow
    auto execute(Trade trade) -> void {
        std::cout << "\t\t$" << trade.price << " * " << trade.quantity << " ";
        if (trade.aggressor_id == trade.buyer_id) {
            std::cout << "[" << trade.buyer_id << "]" << " buys from " << trade.seller_id << std::endl;
        } else {
            std::cout << trade.buyer_id << " buys from [" << trade.seller_id << "]" << std::endl;
        }
    }
};

struct MatchingEngine {
    NaiveOrderbookSide<BidPolicy> bid_book_;
    NaiveOrderbookSide<AskPolicy> ask_book_;

public:
    auto insert_bid(Order order) -> void {
        std::cout << "inserting bid...\n";
        auto remaining = ask_book_.match(order);
        if (remaining.has_value()) {
            bid_book_.insert(remaining.value());
        }
    };

    auto insert_ask(Order order) -> void {
        std::cout << "inserting ask...\n";
        auto remaining = bid_book_.match(order);
        if (remaining.has_value()) {
            ask_book_.insert(remaining.value());
        }
    };
};
