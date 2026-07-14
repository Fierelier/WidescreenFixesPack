#pragma once

#include <stdafx.h>
#include "common.h"
#include "Skeleton.h"

enum FrontendClass
{
    eCDarkel,
    eCMenuManager,
    eLoadingScreen,
    eCRadar,
    eCHud,
    ecMusicManager,
    eConvertingTexturesScreen,
    eCReplay,

    FrontendClassCount
};

struct __declspec(align(4)) CFontDetails
{
    CRGBA m_Color;
    CVector2D m_vecScale;
    float m_fSlant;
    CVector2D m_vecSlantRefPoint;
    bool m_bJustify;
    bool m_bCentre;
    bool m_bRightJustify;
    bool m_bBackground;
    bool m_bBackGroundOnlyText;
    bool m_bProp;
    char _pad1E[2];
    float m_fAlphaFade;
    CRGBA m_BackgroundColor;
    float m_fWrapX;
    float m_fCentreSize;
    float m_fRightJustifyWrap;
    int16_t m_nStyle;
    char _pad36[2];
    int m_nBank;
    int16_t m_nDropShadowPosition;
    CRGBA m_DropColor;
    char _pad42[2];
};

namespace CFont
{
    inline void (__cdecl* SetColor)(CRGBA* color) = nullptr;

    inline GameRef<CFontDetails> Details([]() -> CFontDetails*
    {
        auto pattern = hook::pattern("68 ? ? ? ? 52 0F BF 05");
        if (!pattern.empty())
            return *pattern.get_first<CFontDetails*>(1);
        return nullptr;
    });
}

inline std::array<ProtectedGameRef<float>, FrontendClassCount> ResXInvRefs;
inline std::array<ProtectedGameRef<float>, FrontendClassCount> ResYInvRefs;
