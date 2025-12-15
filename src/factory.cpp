#include "../include/factory.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// PARSING HELPER FUNCTIONS
// Outside of NetSim namespace and hpp files because implementation is not
// needed to know for the library user
struct ParsedLineData {
    std::string element_type;
    std::map<std::string, std::string> parameters;
};

ParsedLineData parse_line(const std::string &line) {
    ParsedLineData data; // create a struct instance

    std::istringstream iss(line);
    std::string token;

    iss >> data.element_type; // first world is an element type

    while (iss >> token) {
    }
    return data;
}

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

// LOAD / SAVE IMPLEMENTATION

Factory load_factory_structure(std::istream &is) {
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == ';', line[0] == '#')
            continue;

        ParsedLineData data = parse_line(line);

        if (data.element_type == "LOADING_RAMP") {
            ElementID id = std::stoi(data.parameters["id"]);
            TimeOffset di = std::stoi(data.parameters["delivery-interval"]);
            factory.add_ramp(Ramp(id, di));
        } else if (data.element_type == "WORKER") {
            ElementID id = std::stoi(data.parameters["id"]);
            TimeOffset pt = std::stoi(data.parameters["processing-time"]);
            std::string qt_str = data.parameters["queue-type"];
            PackageQueueType qt = (qt_str == "LIFO") ? PackageQueueType::LIFO
                                                     : PackageQueueType::FIFO;

            factory.add_worker(
                Worker(id, pt, std::make_unique<PackageQueue>(qt)));
        } else if (data.element_type == "STOREHOUSE") {
            ElementID id = std::stoi(data.parameters["id"]);
            factory.add_storehouse(Storehouse(id));
        } else if (data.element_type == "LINK") {
            std::string src_str = data.parameters["src"];
            std::string dest_str = data.parameters["dest"];

            /**
             * @brief Separetes node-type and node-id
             * input format is: <node-type>-<node-id>
             */
            auto parse_node_str = [](const std::string &s)
                -> std::pair<std::string, ElementID> { // node type, element id
                size_t dash_pos = s.find("-");
                return {s.substr(0, dash_pos),
                        std::stoi(s.substr(dash_pos + 1))};
            };

            auto [src_type, src_id] = parse_node_str(src_str);
            auto [dest_type, dest_id] = parse_node_str(dest_str);

            PackageSender *sender = nullptr; // pointer to a sender
            if (src_type == "ramp") {
                sender = &(*factory.find_ramp_by_id(src_id)); //
            } else if (src_type == "worker") {
                sender = &(*factory.find_worker_by_id(src_id));
            }

            IPackageReceiver *receiver = nullptr; // pointer to a receiver
            if (dest_type == "worker") {
                receiver = &(*factory.find_worker_by_id(dest_id));
            } else if (dest_type == "storehouse") {
                receiver = &(*factory.find_storehouse_by_id(dest_id));
            }

            if (sender && receiver) {
                sender->get_receiver_preferences().add_receiver(
                    receiver); // passing a pointer to the receiver to add to
                               // sender's preferences
            }
        }
    }

    return factory;
}

void save_factory_structure(Factory &factory, std::ostream &os) {}

} // namespace NetSim