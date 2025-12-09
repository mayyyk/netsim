#include "../include/factory.hpp"
#include <iostream>
#include <stdexcept>

namespace NetSim {
enum class NodeColor { UNVISITED, VISITED, VERIFIED };

// FACTORY IMPLEMENTATION

// bool has_reachable_storehouse() {}

// bool Factory::is_consistent() {
//     std::map<PackageSender *, NodeColor> node_colors;

//     // Initializing colors for Workers and Ramps - all sending nodes
//     for (const auto &ramp : ramps_) {
//         node_colors[&ramp] = NodeColor::UNVISITED;
//     }
//     for (const auto &worker : workers_) {
//         node_colors[&worker] = NodeColor::UNVISITED;
//     }

//     try {
//         for (const auto &ramp : ramps_) {
//             has_reachable_storehouse(&ramp, node_colors);
//         }
//     } catch (const std::logic_error &e) {
//         return false;
//     }

//     return true;
// }

void Factory::do_deliveries(Time t) {
    for (auto &ramp : ramps_) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto &ramp : ramps_) {
        ramp.send_package();
    }

    for (auto &worker : workers_) {
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto &worker : workers_) {
        worker.do_work(t);
    }
}

void Factory::remove_worker(ElementID id) {
    auto it = workers_.find_by_id(id);
    if (it == workers_.end())
        return;

    Worker *worker_to_remove =
        &(*it); // changing smart pointer (iterator) to an ordinary pointer

    remove_receiver_from_senders(ramps_, worker_to_remove);

    remove_receiver_from_senders(workers_, worker_to_remove);

    workers_.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id) {
    auto it = storehouses_.find_by_id(id);
    if (it == storehouses_.end())
        return;

    Storehouse *store_to_remove = &(*it);

    remove_receiver_from_senders(ramps_, store_to_remove);
    remove_receiver_from_senders(workers_, store_to_remove);

    storehouses_.remove_by_id(id);
}

} // namespace NetSim