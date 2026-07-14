#include <stdafx.h>
#include "common.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // needed for timeBeginPeriod()/timeEndPeriod()

#include "Skeleton.h"
#include "Menu.h"

namespace
{

namespace CText
{
    void* TheText = nullptr;
    const char* (__fastcall* Get)(void*, void*, const char*) = nullptr;

    auto CUTSCENE_BORDERS = "CUTSCENE BORDERS";

    const char* GetCutsceneBordersText()
    {
        switch (FrontendMenuManager->m_bWidescreenOn)
        {
            case CutsceneBordersMode::Off:
                return Get(TheText, 0, "FEM_OFF");
            case CutsceneBordersMode::Letterbox:
                return "LETTERBOX";
            case CutsceneBordersMode::Pillarbox:
                return "PILLARBOX";
            case CutsceneBordersMode::Both:
                return "LETTERBOX + PILLARBOX";
            default:
                break;
        };

        return Get(TheText, 0, "FEM_ON");
    }

    const char* GetFrameLimiterText()
    {
        switch (FrontendMenuManager->m_bPrefsFrameLimiter)
        {
            case FrameLimiterMode::eOff:
                return Get(TheText, 0, "FEM_OFF");
            case FrameLimiterMode::e30:
                return "30 FPS";
            case FrameLimiterMode::e40:
                return "40 FPS";
            case FrameLimiterMode::e50:
                return "50 FPS";
            case FrameLimiterMode::e60:
                return "60 FPS";
            case FrameLimiterMode::e75:
                return "75 FPS";
            case FrameLimiterMode::e100:
                return "100 FPS";
            case FrameLimiterMode::e120:
                return "120 FPS";
            case FrameLimiterMode::e144:
                return "144 FPS";
            case FrameLimiterMode::e165:
                return "165 FPS";
            case FrameLimiterMode::e200:
                return "200 FPS";
            case FrameLimiterMode::e240:
                return "240 FPS";
            default:
                break;
        };
        return Get(TheText, 0, "FEM_ON");
    }
}

enum RsEvent
{
    rsCAMERASIZE = 0,
    rsCOMMANDLINE = 1,
    rsFILELOAD = 2,
    rsINITDEBUG = 3,
    rsINPUTDEVICEATTACH = 4,
    rsLEFTBUTTONDOWN = 5,
    rsLEFTBUTTONUP = 6,
    rsMOUSEMOVE = 7,
    rsMOUSEWHEELMOVE = 8,
    rsPLUGINATTACH = 9,
    rsREGISTERIMAGELOADER = 10,

    rsRIGHTBUTTONDOWN = 11,
    rsRIGHTBUTTONUP = 12,
    rsMIDDLEBUTTONDOWN = 13,
    rsMIDDLEBUTTONUP = 14,
    rsMOUSEWHEELMOVEDUP = 15,
    rsMOUSEWHEELMOVEDDOWN = 16,
    rsFIRST_XBUTTONUP = 17,
    rsSECOND_XBUTTONUP = 18,
    rsFIRST_XBUTTONDOWN = 19,
    rsSECOND_XBUTTONDOWN = 20,

    rsRWINITIALIZE = 21,
    rsRWTERMINATE = 22,
    rsSELECTDEVICE = 23,
    rsINITIALIZE = 24,
    rsTERMINATE = 25,
    rsIDLE = 26,
    rsFRONTENDIDLE = 27,
    rsKEYDOWN = 28,
    rsKEYUP = 29,
    rsQUITAPP = 30,

    rsPADBUTTONDOWN = 31,
    rsPADBUTTONUP = 32,
    rsPADANALOGUELEFT = 33,
    rsPADANALOGUELEFTRESET = 34,
    rsPADANALOGUERIGHT = 35,
    rsPADANALOGUERIGHTRESET = 36,
    rsPREINITCOMMANDLINE = 37,
    rsACTIVATE = 38,
    rsSETMEMORYFUNCS = 39,
};

class FrameLimiter
{
public:
    enum FPSLimitMode
    {
        FPS_NONE,
        FPS_REALTIME,
        FPS_ACCURATE
    };

    FPSLimitMode mFPSLimitMode = FPS_NONE;

private:
    double TIME_Frequency = 0.0;
    double TIME_Ticks = 0.0;
    double TIME_Frametime = 0.0;
    float fFPSLimit = 0.0f;
    bool bFpsLimitWasUpdated = false;

public:
    void Init(FPSLimitMode mode, float fps_limit)
    {
        bFpsLimitWasUpdated = true;
        mFPSLimitMode = mode;
        fFPSLimit = fps_limit;

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        static constexpr auto TICKS_PER_FRAME = 1;
        auto TICKS_PER_SECOND = (TICKS_PER_FRAME * fFPSLimit);

        if (mFPSLimitMode == FPS_ACCURATE)
        {
            TIME_Frametime = 1000.0 / (double)fFPSLimit;
            TIME_Frequency = (double)frequency.QuadPart / 1000.0;
        }
        else
        {
            TIME_Frequency = (double)frequency.QuadPart / (double)TICKS_PER_SECOND;
        }

        Ticks();
    }

    DWORD Sync_RT()
    {
        if (bFpsLimitWasUpdated)
        {
            bFpsLimitWasUpdated = false;
            return 1;
        }

        DWORD lastTicks, currentTicks;
        LARGE_INTEGER counter;

        QueryPerformanceCounter(&counter);
        lastTicks = (DWORD)TIME_Ticks;
        TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
        currentTicks = (DWORD)TIME_Ticks;

        return (currentTicks > lastTicks) ? currentTicks - lastTicks : 0;
    }

    DWORD Sync_SLP()
    {
        if (bFpsLimitWasUpdated)
        {
            bFpsLimitWasUpdated = false;
            return 1;
        }

        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        double millis_current = (double)counter.QuadPart / TIME_Frequency;
        double millis_delta = millis_current - TIME_Ticks;

        if (TIME_Frametime <= millis_delta)
        {
            TIME_Ticks = millis_current;
            return 1;
        }
        else if (TIME_Frametime - millis_delta > 2.0)
            Sleep(1);
        else
            Sleep(0);

        return 0;
    }

    void Sync()
    {
        if (fFPSLimit <= 0.0f)
            return;

        if (mFPSLimitMode == FPS_REALTIME)
            while (!Sync_RT());
        else if (mFPSLimitMode == FPS_ACCURATE)
            while (!Sync_SLP());
    }

private:
    void Ticks()
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
    }
};

float GetFrameLimiterValue()
{
    switch (FrontendMenuManager->m_bPrefsFrameLimiter)
    {
        case FrameLimiterMode::e30:  return 30.0f;
        case FrameLimiterMode::e40:  return 40.0f;
        case FrameLimiterMode::e50:  return 50.0f;
        case FrameLimiterMode::e60:  return 60.0f;
        case FrameLimiterMode::e75:  return 75.0f;
        case FrameLimiterMode::e100: return 100.0f;
        case FrameLimiterMode::e120: return 120.0f;
        case FrameLimiterMode::e144: return 144.0f;
        case FrameLimiterMode::e165: return 165.0f;
        case FrameLimiterMode::e200: return 200.0f;
        case FrameLimiterMode::e240: return 240.0f;
        case FrameLimiterMode::eOff:
        default:
            return 0.0f;
    }
}

injector::hook_back<bool(__cdecl*)(RsEvent event, void* data)> hbRsEventHandler;
bool __cdecl RsEventHandler(RsEvent event, void* data)
{
    static FrameLimiter fpsLimiter;
    static constexpr auto mode = FrameLimiter::FPS_ACCURATE;
    static bool bTimerResolutionSet = false;
    static float fLastFpsLimit = -1.0f;

    float fFpsLimit = GetFrameLimiterValue();

    if (fFpsLimit != fLastFpsLimit)
    {
        fLastFpsLimit = fFpsLimit;

        if (!bTimerResolutionSet)
        {
            timeBeginPeriod(1);
            bTimerResolutionSet = true;
        }

        RsGlobalFix->maxFPS = static_cast<int32_t>(fFpsLimit);
        fpsLimiter.Init(mode, fFpsLimit);
    }

    fpsLimiter.Sync();

    return hbRsEventHandler.fun(event, data);
}

class Menu
{
public:
    Menu()
    {
        WFP::onGameInitEvent() += []()
        {
            auto pattern = hook::pattern("B9 ? ? ? ? ? ? ? E8 ? ? ? ? 50 E8 ? ? ? ? 83 C4 ? 8B 46");
            CText::TheText = *pattern.get_first<void*>(1);
            CText::Get = (decltype(CText::Get))injector::GetBranchDestination(pattern.get_first(8)).as_int();

            pattern = hook::pattern("8B 4C 24 ? 8B 54 24 ? 53 51 52 E8");
            static auto GetMenuOptionText = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                auto ptr = (char*)(regs.ebx);
                if (ptr)
                {
                    if (std::string_view(ptr) == CText::Get(CText::TheText, 0, "FED_WIS"))
                    {
                        *(char**)&regs.ebx = const_cast<char*>(CText::CUTSCENE_BORDERS);
                    }
                }
            });

            static int cachedAction = -1;
            pattern = hook::pattern("83 C0 E8 83 F8 29");
            static auto GetCurrentAction = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                cachedAction = regs.eax;
            });

            pattern = hook::pattern("85 DB BF C0 01 00 00");
            static auto GetMenuOnOffText = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                auto ptr = (char*)(regs.esi);
                if (ptr)
                {
                    if (cachedAction == 0x18)
                    {
                        if (std::string_view(ptr) == CText::Get(CText::TheText, 0, "FEM_ON") || std::string_view(ptr) == CText::Get(CText::TheText, 0, "FEM_OFF"))
                        {
                            *(char**)&regs.esi = const_cast<char*>(CText::GetFrameLimiterText());
                        }
                    }
                    else if (cachedAction == 0x1A)
                    {
                        if (std::string_view(ptr) == CText::Get(CText::TheText, 0, "FEM_ON") || std::string_view(ptr) == CText::Get(CText::TheText, 0, "FEM_OFF"))
                        {
                            *(char**)&regs.esi = const_cast<char*>(CText::GetCutsceneBordersText());
                        }
                    }
                }
            });

            pattern = hook::pattern("8A 46 ? 84 C0 0F 94 C2 8B CE");
            injector::MakeNOP(pattern.get_first(), 13, true);
            static auto ProcessOnOffMenuOptionsHook = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                regs.ecx = regs.esi;
                auto& pref = FrontendMenuManager->m_bWidescreenOn;

                auto menuDirection = 1;
                if (*(int8_t*)(regs.esp + 0x8) == -1)
                    menuDirection = -1;
                pref += menuDirection;

                if (pref > CutsceneBordersMode::Both)
                    pref = CutsceneBordersMode::Off;
                if (pref < CutsceneBordersMode::Off)
                    pref = CutsceneBordersMode::Both;
            });

            pattern = hook::pattern("8A 46 ? 84 C0 0F 94 C0 8B CE 88 46 ? E8 ? ? ? ? 5F 5E 8A C3");
            injector::MakeNOP(pattern.get_first(), 13, true);
            static auto ProcessOnOffMenuOptionsHook2 = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
            {
                regs.ecx = regs.esi;
                auto& pref = FrontendMenuManager->m_bPrefsFrameLimiter;

                auto menuDirection = 1;
                if (*(int8_t*)(regs.esp + 0x8) == -1)
                    menuDirection = -1;
                pref += menuDirection;

                if (pref > FrameLimiterMode::e240)
                    pref = FrameLimiterMode::eOff;
                if (pref < FrameLimiterMode::eOff)
                    pref = FrameLimiterMode::e240;
            });

            pattern = hook::pattern("75 ? 80 3D ? ? ? ? ? 74 ? B9");
            injector::MakeNOP(pattern.get_first(), 2, true);

            pattern = hook::pattern("7A ? 57 6A ? E8");
            injector::MakeNOP(pattern.get_first(), 2, true);

            pattern = hook::pattern("E8 ? ? ? ? 83 C4 ? 39 1D ? ? ? ? 75 ? 89 3D");
            hbRsEventHandler.fun = injector::MakeCALL(pattern.get_first(0), RsEventHandler).get();
        };
    }
} Menu;

}
