#pragma once

#include "factory.hpp"
#include "types.hpp"
#include <functional>

namespace NetSim {
    /**
     * @brief Main function controlling the simulation
     * @param f Factory reference (network model)
     * @param rounds Amound of simulation rounds
     * @param rf reporting function, called in every round
     */
    void simulate(Factory &f, Time rounds, std::function<void(Factory&, Time)> rf);
}