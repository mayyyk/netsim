#include "../include/reports.hpp"

namespace NetSim {
    SpecificRoundsReportNotifier::SpecificRoundsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)){}

    bool SpecificRoundsReportNotifier::should_generate_report(Time t) const {
        return turns_.find(t) != turns_.end(); // check if the current round (t) is in the set of turns (turns_), only if the set is not finished (simulation end)

    }

    IntervalReportNotifier::IntervalReportNotifier(TimeOffset to) : to_(std::move(to)) {}

    IntervalReportNotifier::should_generate_report(Time t) const {
        
    }
    
}
