#pragma once
#include "types.hpp"

namespace NetSim
{
    class PackageSender
    {
    public:
    };
    class IPackageReceiver
    {
    };

    class ReceiverPreferences
    {
    };

    class Ramp : public PackageSender
    {
    };

    class Worker : public PackageSender, public IPackageReceiver
    {
    };

    class Storehouse : public IPackageReceiver {

    };

} // NetSim namespace