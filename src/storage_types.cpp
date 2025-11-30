#include "../include/storage_types.hpp"

namespace NetSim
{

    PackageQueue::PackageQueue(PackageQueueType type) : queue_type_(type) {}

    void PackageQueue::push(Package &&package)
    {
        // Element should always be places at the back
        deque_.emplace_back(std::move(package)); // std::move only moves the content of "package", "package" remains empy and is removed at the end of scope
    }

    bool PackageQueue::empty() const
    {
        return deque_.empty();
    }

    size_t PackageQueue::size() const
    {
        return deque_.size();
    }

    PackageQueueType PackageQueue::get_queue_type() const
    {
        return queue_type_;
    }

    Package PackageQueue::pop()
    {
        Package p; // to temporarily store front element of deque_, so we can clean the memory

        if (queue_type_ == PackageQueueType::FIFO)
        {
            p = std::move(deque_.front()); // Content of the deque_.front is now in "p" but deque_.front still exist (as an empty container)
            deque_.pop_front(); // Remove the empty container
        }
        else if (queue_type_ == PackageQueueType::LIFO)
        {
            p = std::move(deque_.back());
            deque_.pop_back();
        }

        return p;
    }
} // namespace NetSim