#pragma once

#include <stdafx.h>
#include <d3d9.h>

#include "Skeleton.h"

struct ScreenVertex
{
    float x, y, z, rhw;
    DWORD color;
    float u, v;
};

#define D3DFVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

class TransparentMenuDX9
{
private:
    static inline IDirect3DTexture9* pSceneTex = nullptr;
    static inline IDirect3DSurface9* pSceneSurf = nullptr;

    static inline UINT nScreenWidth = 0;
    static inline UINT nScreenHeight = 0;
    static inline bool bInitialized = false;
    static inline bool captureReady = false;

    static bool IsDeviceReady()
    {
        if (!(*pD3D9Device)) return false;
        const HRESULT hr = (*pD3D9Device)->TestCooperativeLevel();
        return hr == D3D_OK;
    }

    static bool Initialize();
    static void ReleaseResources();

public:
    static bool CaptureFrame();
    static void RenderBlur();
    static void OnDeviceReset();
};
