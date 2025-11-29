#pragma once

#include <list>
#include <deque>
#include "package.hpp"

namespace NetSim
{
    /**
     * @brief Enumeration type defining a quque type
     */
    enum class PackageQueueType
    {
        FIFO,
        LIFO
    };

    /**
     * @brief Interface for any product container
     * Defines basic operations: add, check state that all container types use: WorkerQueue, Storehouse
     * Implementation of storing a Package is not included here becase worker may store it in std::queue
     * and Storehouse just in std::vector
     */
    class IPackageStockpile
    {
    public:
        /**
         * @brief Always add destructor to polimorphic interfaces
         */
        virtual ~IPackageStockpile() {}

        /**
         * @brief Add a product to the container
         */
        virtual void push(Package &&package) = 0;

        /**
         * @brief Check if container is empty
         */
        virtual bool empty() const = 0;

        /**
         * @brief Return the amount of elements in a container (size_t is preferred type for sizes)
         */
        virtual size_t size() const = 0;
    };

    /**
     * @brief Interface for any product queue
     * Inherits from IPackageStockpile - every Queue is a Stockpile
     * Adds method for retrieving a Package from the container according to a defined strategy (FIFO, LIFO)
     */
    class IPackageQueue : public IPackageStockpile
    {
    public:
        /**
         * @brief Rethrievs an element from a queue
         * Order depends on implmentation (FIFO/LIFO)
         * @return Package (product)
         */
        virtual Package pop() = 0;

        /**
         * @brief Returns Queue type
         * @return Enumerate type PackageQueueType
         */
        virtual PackageQueueType get_queue_type() const = 0;

        virtual ~IPackageQueue() = default;
    };

    /**
     * @brief Specifc Queue implementation, handles both FIFO and LIFO
     */
    class PackageQueue : public IPackageQueue
    {
    public:
        /**
         * @brief Constructor
         * @param type PackageQueueType (FIFO or LIFO)
         */
        explicit PackageQueue(PackageQueueType type);

        void push(Package &&package) override;
        bool empty() const override;
        size_t size() const override;
        Package pop() override;
        PackageQueueType get_queue_type() const override;

        ~PackageQueue() override = default;

    private:
        PackageQueueType queue_type;
        std::deque<Package> deque_; // std::deque is ideal because it allows for fast access both from front and back side
    };

} // namespace NetSim
