#pragma once

#include <iterator>
#include <memory>

namespace mtra {

template <typename Iterator>
class reverse_iterator {
// aliases
public:
    using iterator_type = Iterator;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using reference = typename std::iterator_traits<Iterator>::reference;

// member variables
private:
    iterator_type current_;

// resource management
public:
    reverse_iterator() = default;
    explicit reverse_iterator(iterator_type iter) : current_{iter} {} 

    // "copy constructor" for when base iterator U can be converted to T
    template <typename U>
    reverse_iterator(const reverse_iterator<U>& other) : current_{other.base()} {}

// member functions and operator overloads
public:
    auto base() const -> iterator_type {
        return current_;
    }

    auto operator*() const -> reference {
        iterator_type tmp(current_);
        return *--tmp;
    }

    auto operator->() const -> pointer {
        iterator_type tmp(current_);
        return std::addressof(operator*());
    }

    auto operator++() -> reverse_iterator& {
        --current_;
        return *this;
    }

    auto operator++(int dummy) -> reverse_iterator& {
        return normal_iterator(current_++);
    }

    auto operator--() -> reverse_iterator& {
        ++current_;
        return *this;
    }

    auto operator--(int dummy) -> reverse_iterator& {
        return normal_iterator(current_++);
    }

};

}
