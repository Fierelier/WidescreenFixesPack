#pragma once

#include "stdafx.h"

// Public interface formerly exposed via `export module Compat;`.
// XtendedInputCompat and the Compat registration class are implementation
// detail and stay inside Compat.cpp.

namespace XtendedInput
{
    extern HMODULE mhXtendedInput;
    extern float(__cdecl* SetFEScale)(float val);
    extern bool(__cdecl* GetUseWin32Cursor)();
    extern bool bLookedForXInput;
    extern bool bFoundXInput;
}
