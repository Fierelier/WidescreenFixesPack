#pragma once

#include <stdafx.h>
#include "common.h"

inline bool g_noBorderAnim = false;
inline bool g_isMouseCursor = false;
inline bool g_wantsToMoveHudLeft = false;
inline bool g_wantsToMoveHudRight = false;
inline bool g_needsToMoveHudLeft = false;
inline bool g_needsToMoveHudRight = false;
inline SafetyHookInline shDrawRect1 = {};
