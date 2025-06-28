#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

template <typename T>
class spsc_queue_lockfree {
    T buffer_[];
};
