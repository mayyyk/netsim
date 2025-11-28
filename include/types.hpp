// aliases

#pragma once

#include <functional>

namespace NetSim
{
    /**
     * @brief Alias for Nodes and Products ID type
     */
    using ElementID = int;

    /**
     * @brief Alias for probability (0.0 - 1.0)
     */
    using ProbabilityGenerator = std::function<double()>;

    /**
     * @brief Alias for amount of rounds
     */
    using Time = int;

    /**
     * @brief 
     */
} // namespace NetSim
