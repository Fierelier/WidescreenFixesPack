#include <stdafx.h>
#include <cstdint>
#include "common.h"

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
            auto pattern = hook::pattern("E8 ? ? ? ? 0F B7 C0 83 E0 ? 83 F8 ? 74");
            CGeneral::GetRandomNumber = (decltype(CGeneral::GetRandomNumber))injector::GetBranchDestination(pattern.get_first(0)).as_int();

            pattern = hook::pattern("A1 ? ? ? ? 53 ? ? 6A ? FF 74 24");
            pD3D8Device = *pattern.get_first<IUnknown**>(1);
        };
    }
} Skeleton;

}
