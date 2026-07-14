#pragma once

#include <stdafx.h>

namespace CTimer
{
    inline GameRef<unsigned int> m_snTimeInMilliseconds([]() -> unsigned int*
    {
        auto pattern = hook::pattern("81 3D ? ? ? ? ? ? ? ? 73 ? 83 C4");
        if (!pattern.empty())
            return *pattern.get_first<unsigned int*>(2);
        return nullptr;
    });

    inline GameRef<unsigned int> m_snTimeInMillisecondsPauseMode([]() -> unsigned int*
    {
        auto pattern = hook::pattern("A1 ? ? ? ? 89 85 ? ? ? ? EB 07 89 E9 E8 ? ? ? ? 83 BD ? ? ? ? ? 75");
        if (!pattern.empty())
            return *pattern.get_first<unsigned int*>(1);
        return nullptr;
    });

    inline GameRef<float> ms_fTimeStep([]() -> float*
    {
        auto pattern = hook::pattern("D9 1D ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C6 05");
        if (!pattern.empty())
            return *pattern.get_first<float*>(2);
        return nullptr;
    });
}
