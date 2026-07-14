#include <stdafx.h>
#include "common.h"

#include "Skeleton.h"
#include "Menu.h"
#include "TransparentMenuDX8.h"
#include "TransparentMenu.h"

namespace
{

class TransparentMenu
{
public:
    TransparentMenu()
    {
        WFP::onGameInitEvent() += []()
        {
            CIniReader iniReader("");
            auto bTransparentMenu = iniReader.ReadInteger("GRAPHICS", "TransparentMenu", 0) != 0;

            if (!bTransparentMenu)
                return;

            auto pattern = hook::pattern("E8 ? ? ? ? 83 7C 24 ? ? 59 75 ? 83 C4");
            static auto captureHook = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext&)
            {
                static bool wasMenuActive = false;
                bool isMenuActive = CMenuManager::m_bIsActive;

                if (gTransparentMenuWaitForGameplay)
                {
                    if (isMenuActive)
                    {
                        gTransparentMenuCanRender = false;
                        wasMenuActive = true;
                        return;
                    }

                    gTransparentMenuCanRender = false;
                    wasMenuActive = false;
                    gTransparentMenuWaitForGameplay = false;
                    return;
                }

                if (!isMenuActive)
                {
                    wasMenuActive = false;
                    gTransparentMenuCanRender = false;
                    return;
                }

                if (!wasMenuActive || !gTransparentMenuCanRender)
                {
                    gTransparentMenuCanRender = TransparentMenuDX8::CaptureFrame();
                }

                if (gTransparentMenuCanRender)
                {
                    TransparentMenuDX8::RenderBlur();
                }

                wasMenuActive = true;
            });

            WFP::onBeforeReset() += []()
            {
                gTransparentMenuCanRender = false;
                gTransparentMenuWaitForGameplay = true;

                TransparentMenuDX8::OnDeviceReset();
            };
        };
    }
} TransparentMenu;

}
