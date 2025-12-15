# NetSim - Manual Network Manipulation Guide

This document describes the programming interface of the Factory class, which allows for manual modification of the network structure from main.cpp, either after loading the structure from a file or as an alternative to file loading.

# 1. Adding Nodes

Nodes are added by passing (moving) a prepared object to the appropriate factory method. The Factory takes ownership of the object.

## 1.1. Adding a Ramp (Ramp)

```cpp
// Signature: void add_ramp(Ramp&& ramp);

// Example: Add a ramp with ID=1, delivering goods every 2 rounds.
factory.add_ramp(Ramp(1, 2));
```

## 1.2. Adding a Worker (Worker)

The Worker requires defining an input queue (IPackageQueue). We use std::make_unique to safely transfer ownership of the queue pointer.

```cpp
// Signature: void add_worker(Worker&& worker);

// Example 1: Worker ID=1, processing time=3, FIFO queue.
factory.add_worker(Worker(1, 3, std::make_unique<PackageQueue>(PackageQueueType::FIFO)));

// Example 2: Worker ID=2, processing time=1, LIFO queue.
factory.add_worker(Worker(2, 1, std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
```

## 1.3. Adding a Storehouse (Storehouse)

```cpp
// Signature: void add_storehouse(Storehouse&& storehouse);

// Example: Add a storehouse with ID=1.
factory.add_storehouse(Storehouse(1));
```

# 2. Creating Connections (Links)

Connections in NetSim are established by adding a receiver to the sender's preferences. To do this, we must first locate the correct nodes in the factory.

## 2.1. Node Lookup (Finders)

Use the `find_*_by_id` methods, which return iterators.

```cpp
// Look up nodes by ID
auto ramp_it = factory.find_ramp_by_id(1);
auto worker_it = factory.find_worker_by_id(1);
auto store_it = factory.find_storehouse_by_id(1);

// Verification (recommended practice)
if (ramp_it == factory.ramp_end() || worker_it == factory.worker_end()) {
    throw std::runtime_error("Node not found!");
}
```

## 2.2. Linking Nodes

We retrieve raw pointers to the `PackageSender` (sender) and `IPackageReceiver` (receiver) interfaces from the iterators, and then establish the relationship.

**Example: Link Ramp(1) -> Worker(1)**

```cpp
// Dereferencing the iterator (*) yields the object, we take its address (&)
PackageSender* sender = &(*ramp_it);
IPackageReceiver* receiver = &(*worker_it);

// Add the receiver to the sender's preferences
sender->get_receiver_preferences().add_receiver(receiver);
```

**Example: Link Worker(1) -> Storehouse(1)**

```cpp
PackageSender* worker_sender = &(*worker_it);
IPackageReceiver* store_receiver = &(*store_it);

worker_sender->get_receiver_preferences().add_receiver(store_receiver);
```

# 3. Removing Nodes

Methods to remove nodes from the factory's collections using their IDs.

**WARNING**: Before removing a node, ensure you manually remove all incoming and outgoing connections related to it to prevent "dangling pointers" in other nodes.

```cpp
factory.remove_ramp(1);       // Remove ramp ID=1
factory.remove_worker(1);     // Remove worker ID=1
factory.remove_storehouse(1); // Remove storehouse ID=1
```

# 4. Removing Connections

To remove a connection, the receiver must be removed from the sender's preferences list.

```cpp
auto ramp_it = factory.find_ramp_by_id(1);
auto worker_it = factory.find_worker_by_id(1);

PackageSender* sender = &(*ramp_it);
IPackageReceiver* receiver = &(*worker_it);

sender->get_receiver_preferences().remove_receiver(receiver);
```

# 5. Network Consistency Check

After any manual modification (and before starting the simulation), the network's consistency must be verified.

```cpp
try {
    if (factory.is_consistent()) {
        std::cout << "Network is consistent." << std::endl;
    } else {
        std::cerr << "Network is inconsistent!" << std::endl;
    }
} catch (const std::logic_error& e) {
    // Catches exceptions thrown by the is_consistent implementation
    std::cerr << "Consistency check error: " << e.what() << std::endl;
}
```
