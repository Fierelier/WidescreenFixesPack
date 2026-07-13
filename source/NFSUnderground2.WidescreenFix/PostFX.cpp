
#include <stdafx.h>
#include <d3d9.h>

#include "ComVars.h"


namespace
{

// The "ConsoleGamma" post-FX effect used to be rendered via the D3DX9
// Effects framework (D3DXCreateEffectFromResource, ID3DXEffect, D3DXHANDLE,
// etc.). That's been removed: MinGW/GCC toolchains don't ship an import
// library for D3DX9 (it's a legacy DirectX SDK component, never part of the
// Windows SDK or mingw-w64), so linking against it isn't viable here.
// Everything that only existed to support that effect (CPostFX, its D3D
// resource management, GetHandles/RenderGamma/Initialize/Shutdown, and the
// onEndScene/onBeforeReset hooks) has been removed along with it rather
// than left as dead code referencing types that can no longer compile.
//
// The "ConsoleGamma" ini setting is read below for config-compatibility
// but is currently a no-op. Re-enabling this feature under GCC/MinGW would
// require either a MinGW-compatible D3DX9 import lib, or reimplementing
// the (single-technique) shader effect directly against IDirect3DDevice9
// without D3DX9.

class PostFX
{
public:
    PostFX()
    {
        WFP::onInitEventAsync() += []()
        {
            CIniReader iniReader("");
            bool bConsoleGammaEnabled = iniReader.ReadInteger("GRAPHICS", "ConsoleGamma", 1) != 0;
            (void)bConsoleGammaEnabled; // currently unused: see note above
        };
    }
} PostFX;

} // anonymous namespace
