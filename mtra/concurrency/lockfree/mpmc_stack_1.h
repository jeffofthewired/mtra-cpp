#pragma once

#include <memory>
#include <atomic>

namespace mtra {
namespace lockfree {

// source: Concurrency in Action
// Lock Free Queue
// 
// Requires T to have no throw constructors
// Requires quiescent state where only one thread is within the pop() method
template<typename T>
class mpmc_stack_1 {
private:

    // ensure that construction of node does not lock a mutex
    struct node {
        T data_;
        node *next_;

        // allocate a T on the heap and copy construct into it
        // hmmmm does using std::make_shared lock a mutex?
        node (T const& data) : data_{data} {}
    };

private:
    std::atomic<node *> head_;
    std::atomic<unsigned int> threads_in_pop_;

public:
    void push(T const& data) {
        // now two memory allocations are done
        // one heap allocation for the node
        // another heap allocation for std::make_shared
        node * const new_node = new node(data);

        new_node->next_ = head_.load(std::memory_order_relaxed);
        while (
            !head_.compare_exchange_weak(
                new_node->next_, 
                new_node, 
                std::memory_order_acq_rel,
                std::memory_order_acquire
            )
        );
    }

    T pop() {
        threads_in_pop_.fetch_add(1, std::memory_order_acq_rel);

        node *old_head = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(old_head, old_head->next_));
        auto res = old_head->data_;
        try_reclaim(old_head);
        return res;
    }

private:
    // head of a linked list of nodes to be deleted
    std::atomic<node *> to_be_deleted_;

private:
    static void delete_nodes(node *nodes) {
        while (nodes) {
            node *next = nodes->next_;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node *old_head) {
        if (threads_in_pop_.load(std::memory_order_acq_rel) == 1) {
            // safe to delete the node you just removed
            delete old_head;

            // it might also be safe to delete pending nodes?

            // claim the list to delete (other interfering threads will no longer be allowed to access list)
            
            // meanwhile, other nodes are adding to another list????
            // another node can access the pop function now and obtained to_be_deleted_ and tried to append to it
            node *nodes_to_delete = to_be_deleted_.exchange(nullptr, std::memory_order_acq_rel);

            if (threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                // safe to delete pending (any other popper has done what they needed to do and left)
                // now no one has access to the old to_be_deleted_
                // safe to delete all the nodes in here
                delete_nodes(nodes_to_delete);
            } else if (nodes_to_delete) {
                // not safe to delete pending (other threads are trying to append)
                chain_pending_nodes(nodes_to_delete);

                // to_be_deleted is null right now 
                // umm shouldn't we do something about this?
            } else {
                // 
            }
        } else {
            chain_pending_node(old_head);
            threads_in_pop_.fetch_sub(1, std::memory_order_acq_rel);
        }

    }

    void chain_pending_nodes(node *nodes) {
        node *last = nodes;
        while (node *const next = last->next_) last = next;
        chain_pending_nodes(nodes, last);
    }

    void chain_pending_nodes(node *first, node *last) {
        last->next_ = to_be_deleted_;
        while (!to_be_deleted_.compare_exchange_weak(last->next_, first, std::memory_order_acq_rel, std::memory_order_acquire));
    }

    void chain_pending_node(node *n) {
        chain_pending_nodes(n, n);
    }
};

}}
