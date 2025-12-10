#include "reports.hpp"

namespace NetSim {

// --- SPECIFIC TURN-BASED REPORTS NOTIFIER ---

SpecificRoundsReportNotifier::SpecificTurnsReportNotifier(std::set<Time> turns)
    : turns_(std::move(turns)) {}

bool SpecificTurnsReportNotifier::should_generate_report(Time t) const {
    // Sprawdzamy czy bieżąca tura 't' znajduje się w zbiorze
    return turns_.find(t) != turns_.end();
}

// --- INTERVAL-BASED REPORTS NOTIFIER ---

IntervalReportNotifier::IntervalReportNotifier(TimeOffset to)
    : to_(std::move(to)) {}

void IntervalReportNotifier::should_generate_report(Time t) const {
    if t
        == 0 return false;
    return ((t - 1) % to_ == 0);
}

void generate_structure_report(const Factory &f, std::ostream &os) {
    os << " [Mock] Generating Structure Report..." << std::endl;
}

void generate_simulation_turn_report(const Factory &f, std::ostream &os,
                                     Time t) {
    os << " [Mock] Generating Turn Report for turn: " << t << std::endl;
}

} // namespace NetSim