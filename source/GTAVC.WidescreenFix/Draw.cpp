#include <stdafx.h>
#include "common.h"

#include "Draw.h"

CDrawFix::CDrawFix()
{
    WFP::onGameInitEvent() += []()
    {
        auto pattern = hook::pattern("D8 0D ? ? ? ? 8A 44 24 ? 88 5C 24");
        ms_fFOV.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("FF 35 ? ? ? ? 50 A1 ? ? ? ? 6A ? 50 E8 ? ? ? ? A1 ? ? ? ? 83 C4 ? 50 E8 ? ? ? ? A1 ? ? ? ? 59 6A");
        ms_fAspectRatio.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("E8 ? ? ? ? 59 C7 05 ? ? ? ? ? ? ? ? 6A 00 E8");
        static auto SetFOV = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CDrawFix::SetFOV);

        pattern = hook::pattern("E8 ? ? ? ? DD D8 D9 05 ? ? ? ? D8 0D ? ? ? ? D9 F2");
        static auto CalculateAspectRatio = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CDrawFix::CalculateAspectRatio);

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 80 E4 05 80 FC 01 75 ? 30 C0 E9 F2 01 00 00");
        ms_fNearClipZ.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 F6 C4 45 75 ? 30 C0 E9 D6 01 00 00");
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
    float reduction = TheCamera->m_ScreenReductionPercentage;
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
    if (!CCutsceneMgr::IsRunning() && !TheCamera->m_WideScreenOn)
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
            auto pattern = hook::pattern("D9 05 ? ? ? ? D8 0D ? ? ? ? D8 0D ? ? ? ? DD DC D9 C3");
            injector::WriteMemory(pattern.get_first(2), &fScaledFOV, true);
            injector::MakeNOP(pattern.get_first(6), 12, true);
            static auto CalculateDerivedValues = safetyhook::create_mid(pattern.get_first(-8), [](SafetyHookContext& regs)
            {
                fScaledFOV = DEGTORAD(CDrawFix::GetScaledFOV() / 2.0f);
            });

            // CBrightLights::Render, render only when far away
            pattern = hook::pattern("C6 44 24 ? ? EB ? ? ? ? ? ? ? DF E0");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(4), 0, true);
        };
    }
} Camera;

}
