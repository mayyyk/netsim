#include "../include/factory.hpp"
#include "../include/nodes.hpp"   // for manual structure manipulation
#include "../include/reports.hpp" // for reporting
#include "../include/simulation.hpp"

#include <fstream>
#include <iostream>

using namespace NetSim;

int main() {

    // === FACTORY STRUCTURE CREATION ===
    // OPENINIG FILE

    std::ifstream structure_file("network_structure.txt");

    if (!structure_file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    Factory factory;

    // READING FROM FILE
    // Function doesn't know if that's a file, it only sees a stream

    try {
        factory = load_factory_structure(structure_file);
    } catch (const std::exception &e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        return 1;
    }

    // MANUAL FACTORY STRUCTURE MODIFICATION
    // Warning (remember that IDs must be different from these in the input
    // file!!!)

    // factory.add_storehouse(Storehouse(1));

    // === FACTORY STRUCTURE CREATION FINISHED ===

    // Structure report (before simulation)

    // console report
    generate_structure_report(factory, std::cout);

    // file report
    std::ofstream structure_report_file("network_structure_report.txt");
    if (structure_report_file.is_open()) {
        generate_structure_report(factory, structure_report_file);
        structure_report_file.close();
        std::cout << "Saved structure report to the file." << std::endl;
    }

    // Simulation with state reporting

    // Notifiers config
    // reports in rounds: 1, 10, 20
    SpecificRoundsReportNotifier specific_notifier({1, 10, 20});

    // reports every 5 rounds
    IntervalReportNotifier interval_notifier(5);

    // Simulation logging file
    std::ofstream sim_state_file("network_simulation_states.txt");

    Time rounds = 30;

    std::cout << "=== NetSim Simulation Start ===" << std::endl;

    try {
        // & means that lambda function hass access to all variables in main
        simulate(factory, rounds, [&](Factory &f, Time t) {
            bool is_specific = specific_notifier.should_generate_report(t);
            bool is_interval = interval_notifier.should_generate_report(t);

            if (is_interval || is_specific) {
                // console reportt
                generate_simulation_turn_report(f, std::cout, t);

                // file report
                generate_simulation_turn_report(f, sim_state_file, t);
            }
        });

    } catch (const std::invalid_argument &e) {
        std::cerr << "Failed to add a node: " << e.what() << std::endl;
    } catch (const std::logic_error &e) {
        std::cerr << "Simulation failed!" << e.what() << std::endl;
        return 1;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Simulation finished." << std::endl;

    if (sim_state_file.is_open())
        sim_state_file.close();
    return 0;
}
