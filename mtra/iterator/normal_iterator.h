#pragma once

// TODO: sub this out with mine later
#include <iterator>

namespace mtra {

template<typename Iterator, typename Container>
class normal_iterator {
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
    normal_iterator() : current_{} {} 
    explicit normal_iterator(iterator_type iter) : current_{iter} {} 

// member functions and operator overloads
public:
    auto base() const -> iterator_type {
        return current_;
    }

    auto operator*() const -> reference {
        return *current_;
    }

    auto operator->() const -> pointer {
        return current_;
    }

    auto operator++() -> normal_iterator& {
        ++current_;
        return *this;
    }

    auto operator++(int dummy) -> normal_iterator& {
        return normal_iterator(current_++);
    }

    auto operator--() -> normal_iterator& {
        --current_;
        return *this;
    }

    auto operator--(int dummy) -> normal_iterator& {
        return normal_iterator(current_--);
    }
};

}
