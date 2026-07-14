#include <stdafx.h>
#include <cstdint>
#include "common.h"

#include "Vehicle.h"

namespace
{

class Vehicle
{
public:
    Vehicle()
    {
        WFP::onGameInitEvent() += []()
        {
            auto pattern = hook::pattern("E8 ? ? ? ? 85 C0 74 ? E8 ? ? ? ? 8B 80 ? ? ? ? 83 F8 ? 75 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? EB");
            FindPlayerVehicle = (decltype(FindPlayerVehicle))injector::GetBranchDestination(pattern.get_first()).as_int();
        };
    }
} Vehicle;

}
