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
            auto pattern = hook::pattern("E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 89 D9");
            FindPlayerVehicle = (decltype(FindPlayerVehicle))injector::GetBranchDestination(pattern.get_first()).as_int();
        };
    }
} Vehicle;

}
