#pragma once

#include <cstddef>

namespace mtra {

template <typename T>
class span {
// aliases
public:
    using size_type = std::size_t;

private:
    T* data_;
    size_type size_;

public:
    span(T* data, size_type size) : data_{data}, size_{size} {}

    auto operator[](size_type i) -> T& {
        return data_[i];
    }

    auto operator[](size_type i) const -> const T& {
        return data_[i];
    }

    auto size() const -> size_type {
        return size_;
    }

    auto begin() -> T* {
        return data_;
    }

    auto end() -> T* {
        return data_ + size_;
    }
};

}
