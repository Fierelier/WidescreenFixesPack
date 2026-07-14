#include <stdafx.h>
#include "common.h"

#include "Skeleton.h"
#include "Camera.h"
#include "Draw.h"

namespace
{

class Cam
{
public:
    Cam()
    {
        WFP::onGameInitEvent() += []()
        {
            auto pattern = hook::pattern("74 ? D9 05 ? ? ? ? D8 0D ? ? ? ? D8 0D");
            struct Process_FollowPedWithMouse
            {
                void operator()(injector::reg_pack& regs)
                {
                    float fTweakFOV = 1.05f;
                    float fAspectRatio = (SCREEN_WIDTH / SCREEN_HEIGHT);
                    float result = fTweakFOV * fAspectRatio;
                    __asm__ __volatile__ ("flds %0" : : "m" (result));
                }
            }; injector::MakeInline<Process_FollowPedWithMouse>(pattern.get_first(0), pattern.get_first(20));

            pattern = hook::pattern("D9 44 24 ? D8 CE D9 1C 24");
            struct Process_FollowPedWithMouse2
            {
                void operator()(injector::reg_pack& regs)
                {
                    float fTweakFOV = 1.05f;
                    float fAspectRatio = (SCREEN_WIDTH / SCREEN_HEIGHT);
                    float Near = *(float*)(regs.esp + 0x50);
                    float tanHalfFOV = Tan(DEGTORAD(70.0f) / 2.0f);
                    float radius = Near * tanHalfFOV * fAspectRatio * fTweakFOV;
                    __asm__ __volatile__ ("flds %0" : : "m" (radius));
                }
            }; injector::MakeInline<Process_FollowPedWithMouse2>(pattern.get_first(0), pattern.get_first(6));
        };
    }
} Cam;

}
