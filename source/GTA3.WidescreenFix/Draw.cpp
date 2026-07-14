#include <stdafx.h>
#include "common.h"

#include "Draw.h"

CDrawFix::CDrawFix()
{
    WFP::onGameInitEvent() += []()
    {
        auto pattern = hook::pattern("D8 35 ? ? ? ? D8 0D");
        ms_fFOV.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("E8 ? ? ? ? 80 7B ? ? 59 75 ? 8B 83");
        static auto SetFOV = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CDrawFix::SetFOV);

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 80 E4 05 80 FC 01 75 ? 30 C0 E9 BC 00 00 00");
        ms_fNearClipZ.SetAddress(*pattern.get_first<float*>(2));

        pattern = hook::pattern("D8 1D ? ? ? ? DF E0 F6 C4 45 75 ? 30 C0 E9 A0 00 00 00");
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
    std::unordered_map<uintptr_t, float> g_FOVMultipliers;
    float g_BaseFOVMultiplier = 1.0f;

    float GetCurrentMultiplier()
    {
        float total = g_BaseFOVMultiplier;
        for (const auto& [hash, mult] : g_FOVMultipliers)
            total *= mult;
        return total;
    }
}

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
            auto pattern = hook::pattern("D9 05 ? ? ? ? D8 0D ? ? ? ? D8 0D ? ? ? ? DD DC");
            injector::WriteMemory(pattern.get_first(2), &fScaledFOV, true);
            injector::MakeNOP(pattern.get_first(6), 12, true);
            static auto CalculateDerivedValues = safetyhook::create_mid(pattern.get_first(-8), [](SafetyHookContext& regs)
            {
                fScaledFOV = DEGTORAD(CDrawFix::GetScaledFOV() / 2.0f);
            });

            static float fAspectRatio = 0.0f;
            pattern = hook::pattern("80 3D ? ? ? ? ? 74 ? D9 05 ? ? ? ? EB 06");
            injector::WriteMemory(pattern.get_first(11), &fAspectRatio, true);
            injector::WriteMemory(pattern.get_first(19), &fAspectRatio, true);
            static auto Find3rdPersonCamTargetVector = safetyhook::create_mid(pattern.get_first(0), [](SafetyHookContext& regs)
            {
                fAspectRatio = CDrawFix::GetAspectRatio();
            });

            pattern = hook::pattern("74 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? EB ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 80 7B");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(0), 0xEB, true);

            pattern = hook::pattern("C6 44 24 ? ? EB ? ? ? ? ? ? ? DF E0");
            if (!pattern.empty())
                injector::WriteMemory<uint8_t>(pattern.get_first(4), 0, true);
        };
    }
} Camera;

}
