#pragma once

#include <stdafx.h>
#include <cstdint>
#include "common.h"
#include <d3d9.h>
#include "Entity.h"

enum RsInputDeviceType : int
{
    rsKEYBOARD,
    rsMOUSE,
    rsPAD
};

enum RsEventStatus : int
{
    rsEVENTERROR,
    rsEVENTPROCESSED,
    rsEVENTNOTPROCESSED
};

enum RsEvent : int
{
    rsCAMERASIZE,
    rsCOMMANDLINE,
    rsFILELOAD,
    rsINITDEBUG,
    rsINPUTDEVICEATTACH,
    rsLEFTBUTTONDOWN,
    rsLEFTBUTTONUP,
    rsMOUSEMOVE,
    rsMOUSEWHEELMOVE,
    rsPLUGINATTACH,
    rsREGISTERIMAGELOADER,
    rsRIGHTBUTTONDOWN,
    rsRIGHTBUTTONUP,
    _rs_13,
    _rs_14,
    _rs_15,
    _rs_16,
    _rs_17,
    _rs_18,
    _rs_19,
    _rs_20,
    rsRWINITIALIZE,
    rsRWTERMINATE,
    rsSELECTDEVICE,
    rsINITIALIZE,
    rsTERMINATE,
    rsIDLE,
    rsFRONTENDIDLE,
    rsKEYDOWN,
    rsKEYUP,
    rsQUITAPP,
    rsPADBUTTONDOWN,
    rsPADBUTTONUP,
    rsPADANALOGUELEFT,
    rsPADANALOGUELEFTRESET,
    rsPADANALOGUERIGHT,
    rsPADANALOGUERIGHTRESET,
    rsPREINITCOMMANDLINE,
    rsACTIVATE,
};

using RsInputEventHandler = RsEventStatus(*)(RsEvent event, void* param);

struct RsInputDevice
{
    RsInputDeviceType inputDeviceType;
    bool              used;
    RsInputEventHandler inputEventHandler;
};

struct RsGlobalType
{
    const char* appName;
    union
    {
        int32_t maximumWidth;
        int32_t width;
    };
    union
    {
        int32_t maximumHeight;
        int32_t height;
    };
    int32_t maxFPS;
    bool  quit;
    void* ps;
    RsInputDevice keyboard;
    RsInputDevice mouse;
    RsInputDevice pad;
};

inline GameRef<RsGlobalType> RsGlobalFix([]() -> RsGlobalType*
{
    auto pattern = hook::pattern("A1 ? ? ? ? 51 2B 54 24");
    if (!pattern.empty())
        return *pattern.get_first<RsGlobalType*>(1);
    return nullptr;
});

namespace CGeneral
{
    inline unsigned int __cdecl GetRandomNumber()
    {
        return rand();
    }
}

inline CEntity* (__cdecl* FindPlayerPed)(int number) = nullptr;
inline CEntity* (__cdecl* FindPlayerVehicle)(int playerNum, bool bIncludeRemote) = nullptr;
inline void (__cdecl* RwCameraSetNearClipPlane)(RwCamera* camera, float nearClip) = nullptr;

inline IDirect3DDevice9** pD3D9Device = nullptr;

inline GameRef<RwGlobals*> RwEngineInstance;

inline void RwRenderStateSet(RwRenderState nState, void* pParam)
{
    RwEngineInstance->dOpenDevice.fpRenderStateSet(nState, pParam);
}
