#pragma once

#include <limits>
#include <memory>
#include <new>
#include <type_traits>

namespace mtra {

template<typename T, typename A = std::allocator<T>>
class vector {
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
        data_{std::allocator_traits<A>::allocate(alloc_, k_init_capacity)},
        capacity_{k_init_capacity}, 
        size_{0}
    {}

    ~vector() {
        for (std::size_t i = 0; i < size_; ++i) {
            std::allocator_traits<A>::destroy(alloc_, data_ + i);
        }
        std::allocator_traits<A>::deallocate(alloc_, data_, capacity_);
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
        auto new_data = std::allocator_traits<A>::allocate(alloc_, new_capacity);
        for (std::size_t i = 0; i < size_; ++i) {
            std::allocator_traits<A>::construct(alloc_, new_data + i, data_[i]);
            std::allocator_traits<A>::destroy(alloc_, data_ + i);
        }
        std::allocator_traits<A>::deallocate(alloc_, data_, capacity_);
        capacity_ = new_capacity;
    }

    template <typename U>
    auto push_back(U&& value) -> void {
        emplace_back(std::forward<U>(value));
    }

    template<typename ...Args>
    auto emplace_back(Args&&... args) -> void {
        if (size_ == capacity_) [[unlikely]] reserve(capacity_ * k_growth_factor);
        std::allocator_traits<A>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        ++size_;
    }

// essential operator overloads
public:
    auto operator[](std::size_t index) -> T& {
        return data_[index];
    }

};

}
