
#include <stdafx.h>
#include <d3d9.h>

#include "ComVars.h"

#include "Resolution.h"

// Internal linkage: this file's contents were a non-exported module
// purview under C++20 modules and must stay private to this translation
// unit now that it's a plain .cpp, to avoid symbol collisions with other
// files (e.g. two files each defining their own `Init()`).
namespace
{

void StartHUDScissorTest(IDirect3DDevice9* pDevice, float safeAspectRatio, float windowAspectRatio)
{
    if (!pDevice || windowAspectRatio <= 0.0f) return;

    UINT screenWidth = 0;
    UINT screenHeight = 0;

    IDirect3DSurface9* pBackBuffer = nullptr;
    if (SUCCEEDED(pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)) && pBackBuffer)
    {
        D3DSURFACE_DESC desc = {};
        pBackBuffer->GetDesc(&desc);
        screenWidth = desc.Width;
        screenHeight = desc.Height;
        pBackBuffer->Release();
    }
    else
    {
        D3DVIEWPORT9 vp = {};
        pDevice->GetViewport(&vp);
        screenWidth = vp.Width;
        screenHeight = vp.Height;
    }

    if (screenWidth == 0 || screenHeight == 0)
        return;

    RECT scissorRect = { 0, 0, (LONG)screenWidth, (LONG)screenHeight };

    if (windowAspectRatio > safeAspectRatio)
    {
        LONG desiredWidth = (LONG)(screenWidth * (safeAspectRatio / windowAspectRatio) + 0.5f);
        LONG marginX = ((LONG)screenWidth - desiredWidth) / 2;

        scissorRect.left = marginX;
        scissorRect.right = (LONG)screenWidth - marginX;
    }

    pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    pDevice->SetScissorRect(&scissorRect);
}

void EndHUDScissorTest(IDirect3DDevice9* pDevice)
{
    if (!pDevice) return;

    pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

class UIScissor
{
public:
    UIScissor()
    {
        WFP::onInitEventAsync() += []()
        {
            CIniReader iniReader("");
            bool bFixHUD = iniReader.ReadInteger("MAIN", "FixHUD", 1) != 0;

            if (bFixHUD)
            {
                auto pattern = hook::pattern("A1 ? ? ? ? 74 ? 85 C0");
                static auto ScissorTest1 = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
                {
                    if (nGameState == 3 && !MovieIsPlaying())
                        StartHUDScissorTest(Direct3DDevice, 4.0f / 3.0f, GetAspectRatio());
                });

                pattern = hook::pattern("E9 ? ? ? ? 85 C0 74 ? A1");
                static auto ScissorTestRestore1 = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
                {
                    if (nGameState == 3 && !MovieIsPlaying())
                        EndHUDScissorTest(Direct3DDevice);
                });

                pattern = find_pattern("A1 ? ? ? ? 33 F6 85 C0 7E ? 8D 49", "A1 ? ? ? ? 33 F6 85 C0 7E ? EB ? 8D A4 24 ? ? ? ? 90 8B 04 B5 ? ? ? ? 8B 80");
                static auto ScissorTestRestore2 = safetyhook::create_mid(pattern.get_first(), [](SafetyHookContext& regs)
                {
                    if (nGameState == 3 && !MovieIsPlaying())
                        EndHUDScissorTest(Direct3DDevice);
                });
            }
        };
    }
} UIScissor;

} // anonymous namespace
