#pragma once

#include <stdafx.h>
#include "common.h"

#pragma pack(push, 1)
struct CFontDetails
{
    CRGBA color;
    float scaleX;
    float scaleY;
    float slant;
    float slantRefX;
    float slantRefY;
    uint8_t justify;
    uint8_t centre;
    uint8_t rightJustify;
    uint8_t background;
    uint8_t backgroundOnlyText;
    uint8_t proportional;
    uint8_t bIsShadow;
    uint8_t bFlash;
    uint8_t bBold;
    uint8_t pad41[3];
    float alphaFade;
    CRGBA backgroundColor;
    float wrapX;
    float centreSize;
    float rightJustifyWrap;
    uint16_t style;
    uint8_t bFontHalfTexture;
    uint8_t pad3B_3F[5];
    int16_t dropShadowPosition;
    CRGBA dropColor;
    uint8_t bFlashState;
    uint8_t pad67;
    int nFlashTimer;
    uint8_t anonymous_23;
    uint8_t pad6D_6F[3];
    uint32_t anonymous_25;
};
#pragma pack(pop)

inline int ReplaceTextShadowWithOutlineFix = 0;

namespace CFont
{
    inline void (__cdecl* DrawFonts)() = nullptr;

    inline GameRef<CFontDetails> Details([]() -> CFontDetails*
    {
        auto pattern = hook::pattern("80 3D ? ? ? ? ? 75 ? 80 3D ? ? ? ? ? 75 ? 80 3D ? ? ? ? ? 0F 84");
        if (!pattern.empty())
            return *pattern.get_first<CFontDetails*>(2);
        return nullptr;
    });
}
