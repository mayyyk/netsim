

#pragma once // modern, easy, clean way

#include "types.hpp"

namespace NetSim
{
    /**
     * @brief Class representing a product that's being sent in the net
     */
    class Package
    {
    public:
        /**
         * @brief Default constructor
         */
        Package();

        /**
         * @brief Constructor with a given ID
         */
        explicit Package(ElementID id);

        /**
         * @brief Copy constructor
         * For creating packages that are equivalent to each other
         */
        Package(const Package&) = default;

        /**
         * @brief Moving constructor
         */
        Package(Package&&) = default;

        /**
         * @brief Copy operator
         */
        Package& operator=(const Package&) = default;

        /**
         * @brief Moving operator
         */
        Package& operator=(Package&&) = default;

        /**
         * @brief Read package ID
         * @return ElementID package serial number
         */
        ElementID get_id() const;

        /**
         * @brief Desctructor
         */
        ~Package();

    private:
        ElementID id_;
    };

} // namespace NetSim
