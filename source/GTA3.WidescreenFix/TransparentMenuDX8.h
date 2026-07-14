#pragma once

#include <stdafx.h>
#include "common.h"
#include <dxsdk/dx8/d3d8.h>

#include "Skeleton.h"

struct ScreenVertex
{
    float x, y, z, rhw;
    DWORD color;
    float u, v;
};

#define D3DFVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

class TransparentMenuDX8
{
private:
    struct Resources
    {
        IDirect3DSurface8* captureSurf = nullptr; // D3DPOOL_SYSTEMMEM — safe to CopyRects into
        IDirect3DTexture8* renderTex = nullptr; // D3DPOOL_MANAGED   — safe for SetTexture, no RT state side-effects
        int fbWidth = 0;
        int fbHeight = 0;

        bool initialised = false;
        bool captureReady = false;
    };

    static Resources s_res;

    static IDirect3DDevice8* GetDevice8()
    {
        return *reinterpret_cast<IDirect3DDevice8**>(pD3D8Device);
    }

    static bool IsDeviceReady()
    {
        auto* dev = GetDevice8();
        if (!dev) return false;
        return dev->TestCooperativeLevel() == D3D_OK;
    }

    static void InitResources(IDirect3DDevice8* dev);

public:
    static bool CaptureFrame();
    static void RenderBlur();
    static void OnDeviceReset();
};
