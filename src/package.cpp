#include "../include/package.hpp"

namespace NetSim
{
    Package::Package(){id_ = 1;}

    Package::Package(ElementID id) : id_(id)
    {
    }

    ElementID Package::get_id() const
    {
        return id_;
    }

    Package::~Package() {}

} // namespace NetSim
