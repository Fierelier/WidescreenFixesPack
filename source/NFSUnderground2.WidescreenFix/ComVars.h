#pragma once

#include "stdafx.h"
#include "d3d9.h"

// MSVC allows a non-capturing lambda to implicitly convert to a function
// pointer of any calling convention (__cdecl/__stdcall/__fastcall). GCC/MinGW
// does not: a lambda's conversion operator only targets the default
// convention (__cdecl). Tag lambdas that get cast to a WINAPI (__stdcall)
// function pointer with STDCALL_LAMBDA, placed between the parameter list
// and the trailing return type, e.g.:
//   [](HKEY hKey) STDCALL_LAMBDA -> LSTATUS { ... }
#if defined(__GNUC__) && !defined(_MSC_VER)
    #define STDCALL_LAMBDA __attribute__((stdcall))
#else
    #define STDCALL_LAMBDA
#endif

struct bVector3
{
    float x;
    float y;
    float z;
};

struct bVector4
{
    float x;
    float y;
    float z;
    float w;
};

struct bMatrix4
{
    bVector4 v0;
    bVector4 v1;
    bVector4 v2;
    bVector4 v3;
};

struct MouseData
{
    int   DeltaX = 0;
    int   DeltaY = 0;
    int   Wheel = 0;
};
inline MouseData g_Mouse;

struct StickData
{
    int   RawX = 0;
    int   RawY = 0;
    float X = 0.0f;
    float Y = 0.0f;
};
inline StickData g_RightStick;

inline bool bHideCursorForMouseLook = false;

inline int SimRate = -1;

inline GameRef<HWND> hWnd;
inline GameRef<uint32_t> dwWindowedMode;
inline GameRef<tagRECT> WindowRect;
inline GameRef<int> g_RacingResolution;
inline GameRef<IDirect3DDevice9*> Direct3DDevice;
inline GameRef<uint8_t> bExitFEOptionsSystem;

inline float actualDeltaTime = 0.0f;

namespace cFEng
{
    inline void** pInstance = nullptr;
    inline void (__fastcall* MakeLoadedPackagesDirty)(void*, void*) = nullptr;
}

inline int bStringHash(const char* str)
{
    if (!str)
        return -1;

    int result = -1;
    unsigned char c = *str;

    while (c != 0)
    {
        result = c + 33 * result;
        c = *(++str);
    }

    return result;
}

namespace GameFlowManager
{
    inline int** pRaceCoordinator = nullptr;
    inline bool IsPaused()
    {
        if (pRaceCoordinator && *pRaceCoordinator && **pRaceCoordinator == 6)
            return true;

        if (bExitFEOptionsSystem)
            return true;

        return false;
    }
}

template<typename... Args>
class ResChange : public WFP::Event<Args...>
{
public:
    using WFP::Event<Args...>::Event;
};

inline __declspec(noinline) ResChange<int, int>& onResChange()
{
    static ResChange<int, int> ResChangeEvent;
    return ResChangeEvent;
}
