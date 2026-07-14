#pragma once

#include <stdafx.h>
#include "common.h"
#include "Skeleton.h"

inline bool gbNoIslandLoading = false;
inline bool gbNoInteriorLoading = false;

inline bool bSkipNextPlayerCoords = false;
inline bool bSkipNextPlayerHeading = false;
inline bool bSkipNextFade = false;
inline bool bSkipNextClearArea = false;
inline bool bSkipNextSetAreaVisible = false;
inline bool bSkipNextLoadScene = false;

enum eLevelName
{
    LEVEL_IGNORE = -1,
    LEVEL_GENERIC = 0,
    LEVEL_BEACH,
    LEVEL_MAINLAND,

    NUM_LEVELS
};

enum eAreaName
{
    AREA_MAIN_MAP,
    AREA_HOTEL,
    AREA_MANSION,
    AREA_BANK,
    AREA_MALL,
    AREA_STRIP_CLUB,
    AREA_LAWYERS,
    AREA_COFFEE_SHOP,
    AREA_CONCERT_HALL,
    AREA_STUDIO,
    AREA_RIFLE_RANGE,
    AREA_BIKER_BAR,
    AREA_POLICE_STATION,
    AREA_EVERYWHERE,
    AREA_DIRT,
    AREA_BLOOD,
    AREA_OVALRING,
    AREA_MALIBU_CLUB,
    AREA_PRINT_WORKS
};

namespace CGame
{
    inline GameRef<eLevelName> currLevel([]() -> eLevelName*
    {
        auto pattern = find_pattern("3B 05 ? ? ? ? 75 ? 6A");
        if (!pattern.empty())
            return *pattern.get_first<eLevelName*>(2);
        return nullptr;
    });

    inline GameRef<eAreaName> currArea([]() -> eAreaName*
    {
        auto pattern = find_pattern("83 3D ? ? ? ? ? 74 ? 83 3D ? ? ? ? ? 7E ? E8 ? ? ? ? 84 C0");
        if (!pattern.empty())
            return *pattern.get_first<eAreaName*>(2);
        return nullptr;
    });

    inline GameRef<bool> playingIntro([]() -> bool*
    {
        auto pattern = find_pattern("80 3D ? ? ? ? ? 74 ? 6A 00 E8 ? ? ? ? 30 D2");
        if (!pattern.empty())
            return *pattern.get_first<bool*>(2);
        return nullptr;
    });
}
