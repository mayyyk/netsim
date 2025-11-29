

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
        Package(const Package &) = default;

        /**
         * @brief Moving constructor
         */
        Package(Package &&) = default; // && is moving semantics, prevents copying

        /**
         * @brief Copy operator
         * This object can be safely copied
         */
        Package &operator=(const Package &) = default;

        /**
         * @brief Moving operator
         * This object can be safely moved
         */
        Package &operator=(Package &&) = default; // && is moving semantics, prevents copying

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
