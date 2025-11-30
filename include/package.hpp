

#pragma once // modern, easy, clean way

#include "types.hpp"
#include <set>

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
         * @brief Disable Copy constructor
         * It's prohibited due to copying IDs problems - it cannot happend
         */
        Package(const Package &) = delete;
        
        /**
         * @brief Disable Copy operator
         */
        Package& operator=(const Package &) = delete;

        /**
         * @brief Moving constructor
         */
        Package(Package &&) noexcept; // && is moving semantics, prevents copying

        /**
         * @brief Moving operator
         * This object can be safely moved
         */
        Package& operator=(Package &&) noexcept; // && is moving semantics, prevents copying

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

        // ID Poll
        static std::set<ElementID> assigned_ids_;
        static std::set<ElementID> freed_ids_;
    };

} // namespace NetSim
