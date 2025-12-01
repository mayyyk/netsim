#include "../include/factory.hpp"
#include <iostream>
#include <stdexcept>

namespace NetSim {
enum class NodeColor { UNVISITED, VISITED, VERIFIED };

// FACTORY IMPLEMENTATION

bool has_reachable_storehouse() {}

bool Factory::is_consistent() {
    std::map<PackageSender *, NodeColor> node_colors;

    // Initializing colors for Workers and Ramps - all sending nodes
    for (const auto &ramp : ramps_) {
        node_colors[&ramp] = NodeColor::UNVISITED;
    }
    for (const auto &worker : workers_) {
        node_colors[&worker] = NodeColor::UNVISITED;
    }

    try {
        for (const auto &ramp : ramps_) {
            has_reachable_storehouse(&ramp, node_colors);
        }
    } catch (const std::logic_error &e) {
        return false;
    }

    return true;
}

} // namespace NetSim