#include <stdafx.h>
#include "common.h"

#include "Draw.h"
#include "Camera.h"
#include "Skeleton.h"

namespace
{

SafetyHookInline shCalcScreenCoors = {};
bool __cdecl CalcScreenCoors(const RwV3d* in, RwV3d* out, float* outw, float* outh, bool farclip)
{
    CVector viewvec = TheCamera->m_viewMatrix * *in;
    *out = viewvec;
    if (out->z <= CDrawFix::GetNearClipZ() + 1.0f)
        return false;
    if (out->z >= CDrawFix::GetFarClipZ() && farclip)
        return false;
    float recip = 1.0f / out->z;
    out->x *= SCREEN_WIDTH * recip;
    out->y *= SCREEN_HEIGHT * recip;
    const float fov = 70.0f;
    float fovScale = fov / CDrawFix::GetFOV();

    *outw = CDrawFix::ms_bFixSprites ? (fovScale * recip * SCREEN_HEIGHT) : (fovScale * SCREEN_SCALE_AR(recip) * SCREEN_WIDTH);
    *outh = fovScale * recip * SCREEN_HEIGHT;

    return true;
}

class Sprite
{
public:
    Sprite()
    {
        WFP::onGameInitEvent() += []()
        {
            auto pattern = hook::pattern("E8 ? ? ? ? 83 C4 ? 84 C0 0F 84 ? ? ? ? 0F B7 05");
            shCalcScreenCoors = safetyhook::create_inline(injector::GetBranchDestination(pattern.get_first()).as_int(), CalcScreenCoors);
        };
    }
} Sprite;

}
