
#include <stdafx.h>

#include "ComVars.h"

// Internal linkage: this file's contents were a non-exported module
// purview under C++20 modules and must stay private to this translation
// unit now that it's a plain .cpp, to avoid symbol collisions with other
// files (e.g. two files each defining their own `Init()`).
namespace
{

uint32_t* dword_73645C;
uint8_t* pIsMagazineMode;
void* off_71AA4C;

// MSVC __declspec(naked) + `_asm { ... }` doesn't have a GCC/MinGW
// equivalent keyword; the closest match is __attribute__((naked)) combined
// with a single free-form extended asm block. GCC naked functions may only
// contain asm statements (no other C++ code), same as MSVC. We keep the
// body in Intel syntax via the .intel_syntax noprefix/.att_syntax prefix
// bracketing used elsewhere in this file, and pass the two globals in as
// "m" operands so GCC emits the correctly decorated/addressed symbol
// references instead of us hardcoding a name-mangling/underscore-prefix
// scheme by hand.
//
// The one thing extended-asm operand substitution *doesn't* cover is the
// direct `call SomeFunction` between the two naked functions below: that's
// hand-typed text passed straight to the assembler, so it isn't autoedited
// the way "m"/"r" operands are. On this target (i686-w64-mingw32, 32-bit
// cdecl) every C/C++ symbol the compiler emits gets a leading underscore
// (e.g. `CmpShouldClearSurfaceDuring3DRender` becomes the linker symbol
// `_CmpShouldClearSurfaceDuring3DRender`), so a bare, undecorated name
// in hand-written asm text won't resolve at link time. __USER_LABEL_PREFIX__
// is the portable, standard way to pick up whatever prefix (if any) the
// current target/ABI uses, so the same source works if this is ever built
// for a target without the underscore (e.g. x64) too.
#define WFP_ASM_STR2(x) #x
#define WFP_ASM_STR(x) WFP_ASM_STR2(x)
#define WFP_ASM_CONCAT2(a, b) a##b
#define WFP_ASM_CONCAT(a, b) WFP_ASM_CONCAT2(a, b)
#define WFP_ASM_SYMBOL(name) WFP_ASM_STR(WFP_ASM_CONCAT(__USER_LABEL_PREFIX__, name))
extern "C" void __attribute__((naked)) CmpShouldClearSurfaceDuring3DRender()
{
    __asm__ __volatile__
    (
        ".intel_syntax noprefix\n\t"
        "mov eax, %0\n\t"
        "cmp dword ptr [eax], 1\n\t"
        "jne 1f\n\t"

        // pIsMagazineMode is set if the magazine view screen is open.
        "mov eax, %1\n\t"
        "cmp byte ptr [eax], 0\n\t"

        "1:\n\t"
        "ret\n\t"
        ".att_syntax prefix\n\t"
        :
        : "m" (dword_73645C), "m" (pIsMagazineMode)
    );
}

extern "C" void __attribute__((naked)) MaybeClearSurfaceDuringBackgroundRender()
{
    __asm__ __volatile__
    (
        ".intel_syntax noprefix\n\t"
        "call " WFP_ASM_SYMBOL(CmpShouldClearSurfaceDuring3DRender) "\n\t"
        "jz 1f\n\t"

        // In case 3D rendering won't clear the render surface, it must
        // be done during the 2D background rendering.
        "mov dword ptr [esp + 4], 1\n\t"

        "1:\n\t"
        "mov esi, %0\n\t"
        "ret\n\t"
        ".att_syntax prefix\n\t"
        :
        : "m" (off_71AA4C)
    );
}

class BlackMagazineFix
{
public:
    BlackMagazineFix()
    {
        WFP::onInitEvent() += []()
        {
            CIniReader iniReader("");
            bool bBlackMagazineFix = iniReader.ReadInteger("MISC", "BlackMagazineFix", 0) != 0;

            if (bBlackMagazineFix)
            {
                auto pattern = hook::pattern("E8 ? ? ? ? 83 C4 ? E8 ? ? ? ? A1 ? ? ? ? 33 F6");
                if (pattern.empty())
                    return;

                pattern = hook::pattern("8B F8 A0 ? ? ? ? 84 C0 0F 85");
                pIsMagazineMode = *pattern.get_first<uint8_t*>(3);

                pattern = hook::pattern("83 3D ? ? ? ? 01 8B 77 58");
                dword_73645C = *pattern.get_first<uint32_t*>(2);
                injector::MakeRangedNOP(pattern.get_first(0), pattern.get_first(7));
                injector::MakeCALL(pattern.get_first(0), CmpShouldClearSurfaceDuring3DRender); // 4098A2

                pattern = hook::pattern("BE ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 83 C4 ? 85 C0");
                off_71AA4C = *pattern.get_first<void*>(1);
                injector::MakeCALL(pattern.get_first(0), MaybeClearSurfaceDuringBackgroundRender); // 409CF2
            }
        };
    }
} BlackMagazineFix;

#undef WFP_ASM_STR2
#undef WFP_ASM_STR
#undef WFP_ASM_CONCAT2
#undef WFP_ASM_CONCAT
#undef WFP_ASM_SYMBOL

} // anonymous namespace
