#pragma once

#include <atomic>
#include <vector>

namespace mtra {
namespace lockfree {

constexpr size_t cache_line_size = 64;

// this only works for things that can be copied very quickly
template <typename T, typename A>
class spsc_queue {
public:
    using value_type = T;
    using allocator_type = A;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = typename alloc_traits::size_type;
    using difference_type = typename alloc_traits::difference_type;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;

private:
    const size_type capacity_;
    std::vector<value_type, allocator_type> buf_;

    // accessed by both
    alignas(cache_line_size) std::atomic<size_type> size_;
    // only accessed by producer
    alignas(cache_line_size) std::atomic<size_type> push_cursor_ = 0;
    size_type producer_pop_cursor_ = 0;
    // only accessed by consumer
    alignas(cache_line_size) std::atomic<size_type> pop_cursor_ = 0;
    size_type consumer_push_cursor_ = 0;

// resource management
public:
    auto push(const_reference value) -> bool {
        if (full()) return false;
        buf_[(push_cursor_++) % capacity_] = value; // copy assignment?
        size_.fetch_add(1, std::memory_order_release);
        return true;
    }

    auto pop (reference retval) -> bool {
        if (empty()) return false;
        retval = buf_[(pop_cursor_++) % capacity_]; 
        size_.fetch_sub(1, std::memory_order_release);
        return true;
    }

// capacity and size methods
public:
    // both consumer and producer call this when cached cursors need to be updated
    auto size() -> size_type {
        return size_.load(std::memory_order_acquire);
    }

    // this method is only called by the producer
    auto full() -> bool {
        if (push_cursor_ - producer_pop_cursor_ == capacity_) {
            producer_pop_cursor_ = push_cursor_ - size();
            return push_cursor_ - producer_pop_cursor_ == capacity_;
        } else {
            return false;
        }
    }

    // this method is only called by the consumer
    auto empty() -> bool {
        if (consumer_push_cursor_ - pop_cursor_ == 0) {
            consumer_push_cursor_ = pop_cursor_ + size();
            return consumer_push_cursor_ - pop_cursor_ == capacity_;
        } else {
            return false;
        }
    }
};

}}

