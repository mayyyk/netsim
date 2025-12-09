#include "../include/simulation.hpp"
#include <stdexcept>

namespace NetSim {
void simulate(Factory &f, Time rounds,
              std::function<void(Factory &, Time)> rf) {

    // if (!f.is_consistent()) {
    //     throw std::logic_error("Network is not consistent!");
    // }

    // MAIN SIMULATION LOOP
    for (Time t = 0; t <= rounds; t++) {
        f.do_deliveries(t);

        f.do_package_passing();

        f.do_work(t);

        // rf(f,t);
    }
}

} // namespace NetSim