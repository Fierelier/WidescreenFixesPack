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

enum GameFlowState : int32_t
{
    GAMEFLOW_STATE_NONE = 0x0,
    GAMEFLOW_STATE_LOADING_FRONTEND = 0x1,
    GAMEFLOW_STATE_UNLOADING_FRONTEND = 0x2,
    GAMEFLOW_STATE_IN_FRONTEND = 0x3,
    GAMEFLOW_STATE_LOADING_REGION = 0x4,
    GAMEFLOW_STATE_LOADING_TRACK = 0x5,
    GAMEFLOW_STATE_RACING = 0x6,
    GAMEFLOW_STATE_UNLOADING_TRACK = 0x7,
    GAMEFLOW_STATE_UNLOADING_REGION = 0x8,
    GAMEFLOW_STATE_EXIT_DEMO_DISC = 0x9,
};

struct GameFlowManager
{
    char unk[36];
    GameFlowState CurrentGameFlowState;

    static inline bool (*IsPaused)() = nullptr;
};

inline bool bHideCursorForMouseLook = false;

inline int SimRate = -1;

inline GameRef<HWND> hWnd;
inline GameRef<uint32_t> dwWindowedMode;
inline GameRef<IDirect3DDevice9**> Direct3DDevice;
inline GameRef<int> ResX;
inline GameRef<int> ResY;
inline GameRef<GameFlowManager> TheGameFlowManager;

inline void** pOptions = nullptr;

namespace Sim
{
    namespace Internal
    {
        inline GameRef<float> mLastFrameTime;
    }
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
