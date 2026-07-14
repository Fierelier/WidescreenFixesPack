#include <stdafx.h>
#include <cstdint>
#include "common.h"
#include <d3d9.h>

#include "Skeleton.h"

namespace
{

class Skeleton
{
public:
    Skeleton()
    {
        WFP::onInitEvent() += []()
        {
            auto pattern = hook::pattern("A1 ? ? ? ? 8B 10 6A 00 8D 7B");
            pD3D9Device = *pattern.get_first<IDirect3DDevice9**>(1);

            pattern = find_pattern("E8 ? ? ? ? 8B 80 ? ? ? ? 83 C4 ? 85 C0 74 ? ? ? EB ? 33 C0 8B 40 ? 83 F8 ? 0F 8E");
            FindPlayerPed = (decltype(FindPlayerPed))injector::GetBranchDestination(pattern.get_first()).as_int();

            pattern = find_pattern("E8 ? ? ? ? 83 C4 ? 85 C0 74 ? 83 C0 ? ? ? 8B 50 ? 8B 40 ? 89 4C 24 ? ? ? ? ? ? ? ? ? ? ? 89 54 24");
            FindPlayerVehicle = (decltype(FindPlayerVehicle))injector::GetBranchDestination(pattern.get_first()).as_int();

            pattern = find_pattern("8B 0D ? ? ? ? 56 6A ? FF 51 ? 83 C4");
            RwEngineInstance.SetAddress(*pattern.get_first<RwGlobals**>(2));
        };
    }
} Skeleton;

}
