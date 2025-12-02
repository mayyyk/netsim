#include "../include/storage_types.hpp"
#include <stdexcept> // for throwing runtime_error
namespace NetSim {

PackageQueue::PackageQueue(PackageQueueType type) : queue_type_(type) {}

void PackageQueue::push(Package &&package) {
  // Element should always be places at the back
  deque_.emplace_back(std::move(
      package)); // std::move only moves the content of "package", "package"
                 // remains empy and is removed at the end of scope
}

bool PackageQueue::empty() const { return deque_.empty(); }

size_t PackageQueue::size() const { return deque_.size(); }

PackageQueueType PackageQueue::get_queue_type() const { return queue_type_; }

Package PackageQueue::pop() {
  switch (queue_type_) {
  case PackageQueueType::FIFO: {
    Package p = std::move(
        deque_.front()); // Content of the deque_.front is now in "p" but
                         // deque_.front still exist (as an empty container)
    deque_.pop_front();  // Remove the empty container
    return p;
  }
  case PackageQueueType::LIFO: {
    Package p = std::move(deque_.back());
    deque_.pop_back();
    return p;
  }
  default:
    throw std::runtime_error("Unknown queue type.");
  }
}
// Iterators implementation
IPackageStockpile::const_iterator PackageQueue::begin() const {
  return deque_.begin();
}
IPackageStockpile::const_iterator PackageQueue::end() const {
  return deque_.end();
}
IPackageStockpile::const_iterator PackageQueue::cbegin() const {
  return deque_.cbegin();
}
IPackageStockpile::const_iterator PackageQueue::cend() const {
  return deque_.cend();
}
} // namespace NetSim