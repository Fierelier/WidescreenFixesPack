#pragma once

#include <stdafx.h>
#include "common.h"
#include "Skeleton.h"

enum CutsceneBordersMode : int8_t
{
    Off = 0,
    Letterbox = 1,
    Pillarbox = 2,
    Both = 3
};

enum FrameLimiterMode : int8_t
{
    eOff, e30, e40, e50, e60, e75, e100, e120, e144, e165, e200, e240
};

namespace CMenuManager
{
    inline GameRef<int8_t> m_PrefsUseWideScreen([]() -> int8_t*
    {
        auto pattern = hook::pattern("C6 05 ? ? ? ? ? C6 05 ? ? ? ? ? 8B 45 ? 89 45");
        if (!pattern.empty())
            return *pattern.get_first<int8_t*>(2);
        return nullptr;
    });
}

namespace CMenuManager
{
    inline GameRef<int8_t> m_PrefsFrameLimiter([]() -> int8_t*
    {
        auto pattern = hook::pattern("C6 05 ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C6 05 ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C6 05 ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C6 05 ? ? ? ? ? C6 05");
        if (!pattern.empty())
            return *pattern.get_first<int8_t*>(2);
        return nullptr;
    });

    inline GameRef<int8_t> m_bIsActive([]() -> int8_t*
    {
        auto pattern = hook::pattern("80 3D ? ? ? ? ? 74 ? 80 3D ? ? ? ? ? 0F 85 ? ? ? ? B9");
        if (!pattern.empty())
            return *pattern.get_first<int8_t*>(2);
        return nullptr;
    });
}
