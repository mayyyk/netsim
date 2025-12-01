#pragma once
#include "types.hpp"
#include <map>
namespace NetSim
{
    class ReceiverPreferences
    {
    public:
        using preferences_t = std::map<IPackageReceiver *, double>; // Using map data type for storing probability for choosing a given
        // receiver, with pointers to base class (which Worker and Storehouse derive from)

        using const_iterator = preferences_t::const_iterator;

        /**
         * @brief Method for adding receivers
         * This method allows to keep the class constant - all probabilities always sum to one,
         * regardless of the amount of receivers
         */
        virtual void add_receiver() = 0;

        /**
         * @brief Method for removing receivers
         */
        virtual void remove_receiver() = 0;

        /**
         * @brief Method for getting preferences
         * Returns all current links, in "read-only" mode
         */
        virtual preferences_t get_preferences() = 0;
    };
    class PackageSender
    {
    public:
        ReceiverPreferences receiver_preferences();

        /**
         * @brief Default moving constructor
         */
        PackageSender(PackageSender &&other) = default;
    };
    class IPackageReceiver
    {
    };

    /**
     * @brief Class representing a ramp
     * It doesn't need a queue for delivered products, products are delivered on ramp one by one
     * and instantly moved further - to workers
     */
    class Ramp : public PackageSender
    {
    public:
        /**
         * @brief Default constructor
         */
        Ramp();

        /**
         * @brief Constructor with TimeOffest
         * @arg TimeOffset represents time between product deliveries
         */
        explicit Ramp(NetSim::TimeOffset di);

        /**
         * @brief Method for delivering goods with a set frequency
         * This method is called in every round of the simulation
         * @arg is a current time of the simulation
         */
        void deliver_goods(NetSim::Time t);
    };

    /**
     * @brief Class representing a worker
     * 
     */
    class Worker : public PackageSender, public IPackageReceiver
    {
    };

    class Storehouse : public IPackageReceiver
    {
    };

} // NetSim namespace