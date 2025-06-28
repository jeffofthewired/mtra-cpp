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
    iterator_type base_iter_;

// resource management
public:
    normal_iterator() : base_iter_{} {} 
    explicit normal_iterator(Iterator iter) : base_iter_{iter} {} 

// member functions and operator overloads
public:
    auto base() const -> iterator_type {
        return base_iter_;
    }

    auto operator*() const -> reference {
        return *base_iter_;
    }

    auto operator->() const -> pointer {
        return base_iter_;
    }

    auto operator++() -> normal_iterator& {
        ++base_iter_;
        return *this;
    }

    auto operator++(int dummy) -> normal_iterator& {
        return normal_iterator(base_iter_++);
    }

    auto operator--() -> normal_iterator& {
        --base_iter_;
        return *this;
    }

    auto operator--(int dummy) -> normal_iterator& {
        return normal_iterator(base_iter_--);
    }
};

}
