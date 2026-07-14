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
            auto pattern = hook::pattern("A1 ? ? ? ? 8D 4C 2D");
            pD3D8Device = *pattern.get_first<IUnknown**>(1);
        };
    }
} Skeleton;

}
