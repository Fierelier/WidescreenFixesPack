#pragma once

#include <stdafx.h>
#include "common.h"
#include "Skeleton.h"
#include "CutsceneMgr.h"
#include "Camera.h"

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

inline std::optional<float> fHudAspectRatioConstraint;
inline float fWidescreenHudOffset = 0.0f;

class CDrawFix
{
private:
    static inline GameRef<float> ms_fFOV;
    static inline GameRef<float> ms_fNearClipZ;
    static inline GameRef<float> ms_fFarClipZ;

    static inline float ms_fAspectRatio;
    static inline float ms_fScaledFOV;
public:
    static inline bool ms_bProperScaling = true;
    static inline bool ms_bFixRadar = true;
    static inline bool ms_bFixSprites = true;

    static void SetFOV(float fov);
    static float GetFOV() { return ms_fFOV; }
    static float GetScaledFOV() { return ms_fScaledFOV; }

    static float FindAspectRatio();
    static float ConvertFOV(float fov);
    static float ConvertFOVforCutscene(float fov);
    static float ConvertFOVInverse(float fov);
    static float GetAspectRatio() { return ms_fAspectRatio; }
    static void SetAspectRatio(float ratio)
    {
        if (ms_fAspectRatio == ratio)
            return;

        ms_fAspectRatio = ratio;

        fWidescreenHudOffset = CalculateWidescreenOffset(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, true);
        if (fHudAspectRatioConstraint.has_value())
        {
            float value = fHudAspectRatioConstraint.value();
            if (value < 0.0f || value > (32.0f / 9.0f))
                fWidescreenHudOffset = value;
            else
            {
                value = ClampHudAspectRatio(value, FindAspectRatio());
                fWidescreenHudOffset = CalculateWidescreenOffset(SCREEN_HEIGHT * value, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, true);
            }
        }

        onResChange().executeAll(RsGlobalFix->width, RsGlobalFix->height);
    }
    static float ScaleY(float y);

    static void SetNearClipZ(float nearclip) { ms_fNearClipZ = nearclip; }
    static float GetNearClipZ(void) { return ms_fNearClipZ; }
    static void SetFarClipZ(float farclip) { ms_fFarClipZ = farclip; }
    static float GetFarClipZ(void) { return ms_fFarClipZ; }

    CDrawFix();
};

namespace FOVManager
{
    void __cdecl GetCurrentFOV(float* out);
    void __cdecl SetFOVMultiplier(void* hash, float value);
    void __cdecl RemoveFOVMultiplier(void* hash);
}
