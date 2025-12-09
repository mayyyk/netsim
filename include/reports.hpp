#pragma once

#include "factory.hpp"
#include "types.hpp"
#include <iostream>
#include <set>

namespace NetSim {

// CLASSES DECIDING ABOUT REPORTING

/**
 * @brief Class deciding on reporting based on specific rounds
 */
class SpecificRoundsReportNotifier {
  public:
    /**
     * @brief Constructor
     */
    explicit SpecificRoundsReportNotifier(std::set<Time> rounds);

    /**
     * @brief Decides if a report should be generated at current round
     */
    bool should_generate_report(Time t) const;

  private:
    std::set<Time> rounds_;
};

/**
 * @brief Class deciding on reporting based on interval
 */
class IntervalReportNotifier {
  public:
    explicit IntervalReportNotifier(TimeOffset to);
    bool should_generate_report(Time t) const;

  private:
    TimeOffset to_;
};

// REPORTING FUNCTIONS

void generate_structure_report(const Factory &f, std::ostream &oss);

void generate_simulation_turn_report(const Factory &f, std::ostream &oss,
                                     Time t);

} // namespace NetSim
