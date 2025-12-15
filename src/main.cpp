#include "../include/factory.hpp"
#include "../include/nodes.hpp" // for manual structure manipulation
#include "reports.hpp" // for reporting

#include <fstream>
#include <iostream>

using namespace NetSim;

int main() {
    // OPENINIG FILE

    std::ifstream my_file("network_structure.txt");

    if (!my_file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    // READING FROM FILE
    // Function doesn't know if that's a file, it only sees a stream

    try {
        Factory factory = load_factory_structure(my_file);

        // console version
        generate_structure_report(factory, std::cout);

        // file version
        std::ofstream report_file("raport.txt");
        if (report_file.is_open()) {
            generate_structure_report(factory, report_file);
        }

    } catch (const std::exception &e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }

    // std::cout << "=== NetSim Simulation Start ===" << std::endl;

    // std::cout << "Simulation finished." << std::endl;
    return 0;
}
