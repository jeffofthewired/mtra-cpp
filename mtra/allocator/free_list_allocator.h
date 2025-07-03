#pragma once

#include <cstddef>

namespace mtra {

class free_list_allocator {
private:
    using byte = char;
    using size_type = std::size_t;

    struct header {
        size_type size;
        header *next;
        header *prev;
    };

    constexpr static size_t header_size = sizeof(header);

private:
    byte *data_;
    size_type capacity_;

    header *head_;
    header *tail_;

public:
    free_list_allocator(void *data, size_type capacity) 
    :   data_{static_cast<byte *>(data)},
        capacity_{capacity}
    {
        head_ = reinterpret_cast<header *>(data_);
        head_ = reinterpret_cast<header *>(data_ + capacity_ - header_size);
        auto init_block = reinterpret_cast<header *>(data_ + header_size);
        *head_ = {0, init_block, nullptr};
        *init_block = {capacity_ - 3 * header_size, tail_, head_};
        *tail_ = {0, nullptr, init_block};
    }

    auto allocate(size_type n) -> void * {
        header *curr = head_;
        for (; curr != tail_; curr = curr->next) {
            if (curr->size >= n) break;
        }
        if (curr == tail_) return nullptr;

        // remove block from the free list
        auto prev = curr->prev;
        auto next = curr->next;
        prev->next = next;
        next->prev = prev;
        curr->next = nullptr;
        curr->prev = nullptr;

        // determine if the free space in the chosen block is enough for splitting
        
        // do splitting logic

        return nullptr;
    }

    auto deallocate(void *addr) -> void {

    }

};

}

