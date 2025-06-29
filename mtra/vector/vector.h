#pragma once

#include <memory>

#include "mtra/iterator/normal_iterator.h"
#include "mtra/iterator/reverse_iterator.h"

namespace mtra {

template<typename T, typename A = std::allocator<T>>
class vector {
// aliases
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

    // figure out what llvm means by __wrap_iter<pointer> and __wrap_iter<const_pointer>
    using iterator = normal_iterator<pointer, vector<T, A>>;
    using const_iterator = normal_iterator<const_pointer, vector<T, A>>;
    using reverse_iterator = mtra::reverse_iterator<iterator>;
    using const_reverse_iterator = mtra::reverse_iterator<const_iterator>;

// essential variables, constants and classes
private:
    allocator_type alloc_;
    pointer data_;
    size_type capacity_;
    size_type size_;
    static constexpr size_type k_init_capacity = 16;
    static constexpr size_type k_growth_factor = 2;

// essential resource management
public:
    vector() : 
        alloc_{},
        data_{alloc_traits::allocate(alloc_, k_init_capacity)},
        capacity_{k_init_capacity}, 
        size_{0}
    {}

    ~vector() {
        for (size_type i = 0; i < size_; ++i) {
            alloc_traits::destroy(alloc_, data_ + i);
        }
        alloc_traits::deallocate(alloc_, data_, capacity_);
    }

// essential member functions
public:
    auto size() -> size_type {
        return size_;
    }

    auto capacity() -> size_type {
        return capacity_;
    }

    auto reserve(size_type new_capacity) -> void {
        if (new_capacity <= capacity_) [[unlikely]] return;
        auto new_data = alloc_traits::allocate(alloc_, new_capacity);

        size_type i = 0;
        try {
            for (; i < size_; ++i) {
                alloc_traits::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
            }
        } catch (...) {
            for (size_type j = 0; j < i; ++j) {
                alloc_traits::destroy(alloc_, new_data + j);
            }
            alloc_traits::deallocate(alloc_, new_data, new_capacity);
            throw;
        }

        // clean up the old data
        for (size_type i = 0; i < size_; ++i) {
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
    auto operator[](size_type index) -> T& {
        return data_[index];
    }

// TODO: iterator functions: begin, end, rbegin, rend
public:

    auto begin() -> iterator {
        return iterator(data_);
    }

    auto end() -> iterator {
        return iterator(data_ + size_);
    }

    auto rbegin() -> reverse_iterator {
        return reverse_iterator(iterator(data_ + size_));
    }

    auto rend() -> reverse_iterator {
        return reverse_iterator(iterator(data_));
    }

private: 

};

}
