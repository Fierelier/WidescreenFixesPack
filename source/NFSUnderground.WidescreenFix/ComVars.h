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
inline GameRef<int32_t> nGameState;
inline GameRef<void*> pCurrentMoviePlayer;

inline float actualDeltaTime = 0.0f;

namespace cFEng
{
    inline void** pInstance = nullptr;
    inline void (__stdcall* MakeLoadedPackagesDirty)(void*) = nullptr;
    inline void (__cdecl* PopPackage)(char* a1) = nullptr;
}

inline bool MovieIsPlaying()
{
    return pCurrentMoviePlayer != nullptr;
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
        if (pRaceCoordinator && *pRaceCoordinator && (**pRaceCoordinator == 7 || **pRaceCoordinator == 11))
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

struct alignas(4) MenuScreen
{
    void* __vftable;
    bool mPlaySound;
    unsigned int mDirectionForNextSound;
    const char* PackageFilename;
    ///...
};

template<typename... Args>
class MenuScreenBaseNotify : public WFP::Event<Args...>
{
public:
    using WFP::Event<Args...>::Event;
};

inline __declspec(noinline) MenuScreenBaseNotify<MenuScreen*>& onMenuScreenBaseNotify()
{
    static MenuScreenBaseNotify<MenuScreen*> MenuScreenBaseNotifyEvent;
    return MenuScreenBaseNotifyEvent;
}

struct FEMinNode
{
    void* __vftable;
    FEMinNode* next;
    FEMinNode* prev;
};

enum FEObjType : int32_t
{
    FE_None = 0x0,
    FE_Image = 0x1,
    FE_String = 0x2,
    FE_Model = 0x3,
    FE_List = 0x4,
    FE_Group = 0x5,
    FE_CodeList = 0x6,
    FE_Movie = 0x7,
    FE_Effect = 0x8,
    FE_ColoredImage = 0x9,
    FE_AnimImage = 0xA,
    FE_SimpleImage = 0xB,
    FE_MultiImage = 0xC,
    FE_UserMin = 0x100,
};

struct FEObject : FEMinNode
{
    uint32_t GUID;
    uint32_t NameHash;
    char* pName;
    FEObjType Type;
    uint32_t Flags;
    uint16_t RenderContext;
    uint16_t ResourceIndex;
    uint32_t Handle;
    uint32_t UserParam;
    uint8_t* pData;
    uint32_t DataSize;
    //...
};

struct FEImage : FEObject
{
    unsigned int ImageFlags;
};

inline void* (*CreateResourceFile)(const char* filename, int32_t type, int flags, int file_offset, int file_size) = nullptr;
inline void(__fastcall* ResourceFileBeginLoading)(void* ResourceFile, void* callback, void* callback_param) = nullptr;
inline void (__cdecl* ServiceResourceLoading)() = nullptr;

inline void (__cdecl* FEngSetInvisible)(const char* pkg_name, unsigned int obj_hash) = nullptr;
inline unsigned int (__cdecl* FEHashUpper)(const char* String) = nullptr;
inline unsigned int (__cdecl* FEPkgMgr_FindPackage)(const char* pkg_name) = nullptr;
inline FEObject* (__fastcall* FEngFindObject)(const char* pkg_name, unsigned int obj_hash) = nullptr;
