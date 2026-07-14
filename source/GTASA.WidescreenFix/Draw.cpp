#include <stdafx.h>
#include "common.h"

#include "Draw.h"

CDrawFix::CDrawFix()
{
    WFP::onInitEvent() += []()
    {
        auto pattern = hook::pattern("D9 05 ? ? ? ? 8B 15 ? ? ? ? D8 0D ? ? ? ? 68 AB AA AA 3F");
        ms_fFOV.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("D8 35 ? ? ? ? DE C9 D9 E8");
        ms_fAspectRatio.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("E8 ? ? ? ? 83 C4 ? 53 53 53 B9");
        static auto SetFOV = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CDrawFix::SetFOV);

        pattern = hook::pattern("E8 ? ? ? ? ? ? ? ? ? ? A1 ? ? ? ? ? ? 50 51");
        static auto CalculateAspectRatio = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CDrawFix::CalculateAspectRatio);

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 F6 C4 05 0F 8B ? ? ? ? D9 44 24 ? D8 64 24 ? D8 1D");
        ms_fNearClipZ.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 F6 C4 41 0F 84 ? ? ? ? D9 44 24 ? D8 8E");
        ms_fFarClipZ.SetAddress(*pattern.get_first<float*>(2));
    };
}

float CDrawFix::FindAspectRatio()
{
    return SCREEN_WIDTH / SCREEN_HEIGHT;
}

float CDrawFix::ConvertFOV(float hfov)
{
    float ar1 = DEFAULT_ASPECT_RATIO;
    float ar2 = GetAspectRatio();
    hfov = DEGTORAD(hfov);
    float vfov = Atan(tan(hfov / 2) / ar1) * 2;
    hfov = Atan(tan(vfov / 2) * ar2) * 2;
    return RADTODEG(hfov);
}

float CDrawFix::ConvertFOVforCutscene(float fov)
{
    float reduction = TheCamera->m_fScreenReductionPercentage;
    float contentAspect = (4.0f / 3.0f) / std::max(1.0f - reduction / 100.0f, 0.01f);
    float screenAspect = FindAspectRatio();

    if (screenAspect > contentAspect)
    {
        float hfov = DEGTORAD(fov);
        return RADTODEG(2.0f * Atan(tan(hfov / 2.0f) * screenAspect / contentAspect));
    }
    else
    {
        return fov;
    }
}

float CDrawFix::ConvertFOVInverse(float fov)
{
    const float arFrom = DEFAULT_ASPECT_RATIO;
    const float arTo = GetAspectRatio();

    const float rad = DEGTORAD(fov);
    const float out = 2.0f * Atan(tan(rad * 0.5f) * (arFrom / arTo));
    return RADTODEG(out);
}

namespace FOVManager
{

namespace
{
    // Internal storage: hash -> multiplier
    std::unordered_map<uintptr_t, float> g_FOVMultipliers;
    float g_BaseFOVMultiplier = 1.0f; // fallback

    // Recalculate current total multiplier
    float GetCurrentMultiplier()
    {
        float total = g_BaseFOVMultiplier;
        for (const auto& [hash, mult] : g_FOVMultipliers)
            total *= mult;
        return total;
    }
}

// API Exports
void __cdecl GetCurrentFOV(float* out)
{
    if (out)
        *out = CDrawFix::GetFOV() * GetCurrentMultiplier();
}

void __cdecl SetFOVMultiplier(void* hash, float value)
{
    if (!hash) return;
    uintptr_t key = reinterpret_cast<uintptr_t>(hash);
    g_FOVMultipliers[key] = std::clamp(value, 0.5f, 2.0f);
}

void __cdecl RemoveFOVMultiplier(void* hash)
{
    if (!hash) return;
    uintptr_t key = reinterpret_cast<uintptr_t>(hash);
    g_FOVMultipliers.erase(key);
}

}

void CDrawFix::SetFOV(float fov)
{
    if (!CCutsceneMgr::IsRunning() && !TheCamera->m_bWideScreenOn)
    {
        ms_fScaledFOV = ConvertFOV(fov) * FOVManager::GetCurrentMultiplier();
    }
    else
    {
        ms_fScaledFOV = ConvertFOVforCutscene(fov);
    }
    ms_fFOV = fov;
}

void CDrawFix::CalculateAspectRatio()
{
    SetAspectRatio(CDrawFix::FindAspectRatio());
}

float CDrawFix::ScaleY(float y)
{
    return ms_bProperScaling ? y : y * ((float)DEFAULT_SCREEN_HEIGHT / SCREEN_HEIGHT_NTSC);
}

namespace
{

CDrawFix Draw;

void Find3rdPersonCamTargetVectorFMUL(SafetyHookContext& ctx)
{
    float f = CDrawFix::GetAspectRatio();
    __asm__ __volatile__ ("fmuls %0" : : "m" (f));
}

class Camera
{
public:
    Camera()
    {
        WFP::onGameInitEvent() += []()
        {
            CIniReader iniReader("");
            fHudAspectRatioConstraint = ParseWidescreenHudOffset(iniReader.ReadString("MAIN", "HudAspectRatioConstraint", ""));

            static float fScaledFOV = 0.0f;
            auto pattern = hook::pattern("D9 05 ? ? ? ? 56 D8 0D ? ? ? ? 8B F1");
            injector::WriteMemory(pattern.get_first(2), &fScaledFOV, true);
            injector::MakeNOP(pattern.get_first(7), 6, true);
            static auto CalculateFrustumPlanes = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                fScaledFOV = DEGTORAD(CDrawFix::GetScaledFOV() / 2.0f);
            });

            pattern = hook::pattern("D9 05 ? ? ? ? D8 25 ? ? ? ? DC C0 D8 C9 D9 5C 24");
            static auto Find3rdPersonCamTargetVector = safetyhook::create_mid(pattern.get_first(), Find3rdPersonCamTargetVectorFMUL);

            static float fCameraWidth = 0.01403292f;
            injector::MakeNOP(0x50AD79, 6, true);
            injector::WriteMemory<const void*>(0x50AD59 + 0x2, &fCameraWidth, true);
            injector::WriteMemory<const void*>(0x51498D + 0x2, &fCameraWidth, true);
        };
    }
} Camera;

}
