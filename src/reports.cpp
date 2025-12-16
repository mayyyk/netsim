#include "reports.hpp"

namespace NetSim {

// helper function for sorting receivers by type and ID
void sort_receivers(std::vector<IPackageReceiver *> receivers) {
    std::sort(receivers.begin(), receivers.end(),
              [](IPackageReceiver *a, IPackageReceiver *b) {
                  // FIRST CRITERION
                  // check if they are different types
                  // final order is: worker > storehouse
                  if (a->get_receiver_type() != b->get_receiver_type()) {
                      return a->get_receiver_type() < b->get_receiver_type();
                  }

                  // SECOND CRITERION
                  return a->get_id() < b->get_id();
              });
}

// STRUCTURE REPORT

void generate_structure_report(const Factory &f, std::ostream &os) {
    os << "\n== LOADING RAMPS ==\n\n";

    // assuming insertion by order (to check)

    // for printing receivers for "LOADING_RAMP" and "WORKER" blocks
    auto print_receivers = [&](const PackageSender &sender) {
        const auto &prefs =
            sender.get_receiver_preferences()
                .get_preferences(); // using getter to get preferences from
                                    // sender's preferences
        if (prefs.empty())
            return;

        std::vector<IPackageReceiver *> receivers;

        for (auto &pair : prefs)
            receivers.push_back(pair.first);
        sort_receivers(receivers);
        os << "  Receivers:\n";

        for (auto *r : receivers) {
            std::string type = (r->get_receiver_type() == ReceiverType::WORKER)
                                   ? "worker"
                                   : "storehouse";
            os << "    " << type << " #" << r->get_id() << "\n";
        }
    };

    // ramps
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        os << "LOADING RAMP #" << it->get_id() << "\n";
        os << " Delivery interval: " << it->get_delivery_interval() << "\n";
        print_receivers(*it);
        os << "\n";
    }

    // workers
    os << "\n== WORKERS ==\n\n";
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        os << "WORKER #" << it->get_id() << "\n";
        os << "  Processing time: " << it->get_processing_duration() << "\n";
        std::string q_type =
            (it->get_queue()->get_queue_type() == PackageQueueType::FIFO)
                ? "FIFO"
                : "LIFO";
        os << "  Queue type: " << q_type << "\n";
        print_receivers(*it);
        os << "\n";
    }

    // 3. Storehouses
    os << "\n== STOREHOUSES ==\n\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); ++it) {
        os << "STOREHOUSE #" << it->get_id() << "\n\n";
    }
}

// SIMULATION STATE REPORT

void generate_simulation_turn_report(const Factory &f, std::ostream &os,
                                     Time t) {
    os << "=== [ Turn: " << t << " ] ===\n\n";

    // workers
    os << "== WORKERS ==\n\n";
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); ++it) {
        os << "WORKER #" << it->get_id() << "\n";

        // PBuffer (Processing Buffer)
        os << "  PBuffer: ";
        if (it->get_processing_buffer()) {
            Time pt = t - it->get_package_processing_start_time() + 1;
            os << "#" << it->get_processing_buffer()->get_id()
               << " (pt = " << pt << ")";
        } else {
            os << "(empty)";
        }
        os << "\n";

        // Queue
        os << "  Queue: ";
        if (it->begin() == it->end()) {
            os << "(empty)";
        } else {
            bool first = true;
            for (const auto &pkg : *it) {
                if (!first)
                    os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
        }
        os << "\n";

        // SBuffer (Sending Buffer)
        os << "  SBuffer: ";
        if (it->get_sending_buffer()) {
            os << "#" << it->get_sending_buffer()->get_id();
        } else {
            os << "(empty)";
        }
        os << "\n\n";
    }

    // storehouses

    os << "== STOREHOUSES ==\n\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++) {
        os << "STOREHOUSE #" << it->get_id() << "\n";
        os << "  Stock: ";
        if (it->begin() == it->end()) {
            os << "(empty)";
        } else {
            bool first = true;
            for (const auto &pkg : *it) {
                if (!first)
                    os << ", "; // if it's not a first package, separate the
                                // next one by ", "
                os << "#" << pkg.get_id();
                first = false;
            }
        }
        os << "\n\n";
    }
}

// --- SPECIFIC TURN-BASED REPORTS NOTIFIER ---

SpecificRoundsReportNotifier::SpecificRoundsReportNotifier(std::set<Time> turns)
    : turns_(std::move(turns)) {}

bool SpecificRoundsReportNotifier::should_generate_report(Time t) const {
    // Check if current round is in set turns_
    return turns_.find(t) != turns_.end();
}

// --- INTERVAL-BASED REPORTS NOTIFIER ---

IntervalReportNotifier::IntervalReportNotifier(TimeOffset to)
    : to_(std::move(to)) {}

bool IntervalReportNotifier::should_generate_report(Time t) const {
    if (t == 0)
        return false;
    return ((t - 1) % to_ == 0);
}

} // namespace NetSim