#pragma once

#include <memory>

#include "mtra/container/internal/normal_iterator.h"
#include "mtra/container/internal/reverse_iterator.h"

namespace mtra {

template<typename T, typename A = std::allocator<T>>
class vector {
// aliases
public:
    using alloc_traits = std::allocator_traits<A>;

// essential variables, constants and classes
private:
    A alloc_;
    T *data_;
    std::size_t capacity_;
    std::size_t size_;
    static constexpr std::size_t k_init_capacity = 16;
    static constexpr std::size_t k_growth_factor = 2;

// essential resource management
public:
    vector() : 
        alloc_{},
        data_{alloc_traits::allocate(alloc_, k_init_capacity)},
        capacity_{k_init_capacity}, 
        size_{0}
    {}

    ~vector() {
        for (std::size_t i = 0; i < size_; ++i) {
            alloc_traits::destroy(alloc_, data_ + i);
        }
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }

// essential member functions
public:
    auto size() -> std::size_t {
        return size_;
    }

    auto capacity() -> std::size_t {
        return capacity_;
    }

    auto reserve(std::size_t new_capacity) -> void {
        if (new_capacity <= capacity_) [[unlikely]] return;
        auto new_data = alloc_traits::allocate(alloc_, new_capacity);

        std::size_t i = 0;
        try {
            for (; i < size_; ++i) {
                alloc_traits::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
            }
        } catch (...) {
            for (std::size_t j = 0; j < i; ++j) {
                alloc_traits::destroy(alloc_, new_data + j);
            }
            alloc_traits::deallocate(alloc_, new_data, new_capacity);
            throw;
        }

        // clean up the old data
        for (std::size_t i = 0; i < size_; ++i) {
            alloc_traits::destroy(alloc_, data_ + i);
        }

        if (data_) {
            alloc_traits::deallocate(alloc_, data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_capacity;
    }

    template <typename U>
    auto push_back(U&& value) -> void {
        emplace_back(std::forward<U>(value));
    }

    template<typename ...Args>
    auto emplace_back(Args&&... args) -> void {
        if (size_ == capacity_) [[unlikely]] reserve(capacity_ * k_growth_factor);
        alloc_traits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
    }

// essential operator overloads
public:
    auto operator[](std::size_t index) -> T& {
        return data_[index];
    }

// TODO: iterator functions: begin, end, rbegin, rend
public:
    using iterator = normal_iterator<T*, vector<T>>;

    auto begin() -> iterator {
        return iterator(data_);
    }

    auto end() -> iterator {
        return iterator(data_ + size_);
    }

    auto rbegin() -> reverse_iterator<iterator> {
        return reverse_iterator(iterator(data_ + size_));
    }

    auto rend() -> reverse_iterator<iterator> {
        return reverse_iterator(iterator(data_));
    }

private: 

};

}
