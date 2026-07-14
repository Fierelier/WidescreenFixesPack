#include <stdafx.h>
#include <d3d9.h>

#include "TransparentMenuDX9.h"

void TransparentMenuDX9::ReleaseResources()
{
    captureReady = false;
    bInitialized = false;
    nScreenWidth = 0;
    nScreenHeight = 0;

    if (pSceneSurf) { pSceneSurf->Release(); pSceneSurf = nullptr; }
    if (pSceneTex) { pSceneTex->Release(); pSceneTex = nullptr; }
}

void TransparentMenuDX9::OnDeviceReset()
{
    ReleaseResources();
}

bool TransparentMenuDX9::Initialize()
{
    if (bInitialized) return true;
    if (!IsDeviceReady()) return false;

    IDirect3DSurface9* bb = nullptr;
    if (FAILED((*pD3D9Device)->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb)) || !bb)
        return false;

    D3DSURFACE_DESC desc{};
    bb->GetDesc(&desc);
    bb->Release();

    if (!desc.Width || !desc.Height)
        return false;

    nScreenWidth = desc.Width;
    nScreenHeight = desc.Height;

    HRESULT hr = (*pD3D9Device)->CreateTexture(nScreenWidth, nScreenHeight, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT, &pSceneTex, nullptr);
    if (FAILED(hr) || !pSceneTex) { ReleaseResources(); return false; }

    hr = pSceneTex->GetSurfaceLevel(0, &pSceneSurf);
    if (FAILED(hr) || !pSceneSurf) { ReleaseResources(); return false; }

    bInitialized = true;
    return true;
}

bool TransparentMenuDX9::CaptureFrame()
{
    if (!IsDeviceReady()) return false;
    if (!Initialize()) return false;
    if (!pSceneSurf) return false;

    IDirect3DSurface9* currentRT = nullptr;
    if (FAILED((*pD3D9Device)->GetRenderTarget(0, &currentRT)) || !currentRT)
        return false;

    const HRESULT hr = (*pD3D9Device)->StretchRect(currentRT, nullptr, pSceneSurf, nullptr, D3DTEXF_POINT);
    currentRT->Release();

    captureReady = SUCCEEDED(hr);
    return captureReady;
}

void TransparentMenuDX9::RenderBlur()
{
    if (!IsDeviceReady()) return;
    if (!captureReady || !bInitialized || !pSceneTex) return;

    auto* dev = (*pD3D9Device);

    IDirect3DStateBlock9* state = nullptr;
    if (FAILED(dev->CreateStateBlock(D3DSBT_ALL, &state)) || !state)
        return;

    dev->SetTexture(0, pSceneTex);
    dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    dev->SetRenderState(D3DRS_ZENABLE, FALSE);
    dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dev->SetRenderState(D3DRS_LIGHTING, FALSE);

    dev->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 0.0f, 0);

    const float texelX = 1.0f / static_cast<float>(nScreenWidth);
    const float texelY = 1.0f / static_cast<float>(nScreenHeight);

    constexpr int   kernelSize = 2;
    constexpr float blurStrength = 5.0f;

    const float offsetX = texelX * blurStrength;
    const float offsetY = texelY * blurStrength;

    constexpr int  numSamples = (2 * kernelSize + 1) * (2 * kernelSize + 1);
    const BYTE     weight = static_cast<BYTE>(255 / numSamples);
    const D3DCOLOR sampleColor = D3DCOLOR_ARGB(255, weight, weight, weight);

    dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    dev->SetFVF(D3DFVF_SCREENVERTEX);

    for (int oy = -kernelSize; oy <= kernelSize; ++oy)
    {
        for (int ox = -kernelSize; ox <= kernelSize; ++ox)
        {
            float shiftU = static_cast<float>(ox) * offsetX;
            float shiftV = static_cast<float>(oy) * offsetY;

            ScreenVertex quad[4] =
            {
                {0.0f,                            0.0f,                             0.0f, 1.0f, sampleColor, 0.0f + shiftU, 0.0f + shiftV},
                {static_cast<float>(nScreenWidth), 0.0f,                             0.0f, 1.0f, sampleColor, 1.0f + shiftU, 0.0f + shiftV},
                {static_cast<float>(nScreenWidth), static_cast<float>(nScreenHeight), 0.0f, 1.0f, sampleColor, 1.0f + shiftU, 1.0f + shiftV},
                {0.0f,                            static_cast<float>(nScreenHeight), 0.0f, 1.0f, sampleColor, 0.0f + shiftU, 1.0f + shiftV}
            };

            dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, quad, sizeof(ScreenVertex));
        }
    }

    dev->SetTexture(0, nullptr);

    state->Apply();
    state->Release();
}
