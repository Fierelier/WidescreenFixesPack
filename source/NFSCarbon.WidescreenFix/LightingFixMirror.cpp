#include <stdafx.h>

#include "ComVars.h"

// extern "C" (unmangled names, so the naked function below can reference
// these globals by name from inline asm; naked functions only allow basic
// asm with no operand list, so the symbols must be resolvable directly) and
// volatile (the compiler can't see that the asm block reads these, so
// without volatile it treats the pattern-scan writes below as dead stores
// and optimizes the variables away entirely at -O2, breaking the link).
extern "C" {
    uint32_t* volatile LightingFixUpdateMirrorCave_Exit = (uint32_t*)0x00748C5D;
    uint32_t* volatile sub_713AA0 = (uint32_t*)0x713AA0;
    uint32_t* volatile sub_713A30 = (uint32_t*)0x713A30;
    uint32_t* volatile ptr_dword_AB095C = (uint32_t*)0x00AB095C;
    uint32_t* volatile ptr_dword_AB0914 = (uint32_t*)0x00AB0914;
}

// i686 Windows (32-bit, cdecl) decorates extern "C" symbols with a leading
// underscore at the object-file level (unlike Linux ELF or x86-64 Windows).
// The naked function below uses basic asm with no operand list, so the
// symbol names must be spelled out as literal text and have to include that
// underscore explicitly to match what the compiler actually emits.
extern "C" void __attribute__((naked)) LightingFixUpdateMirrorCave()
{
    __asm__ __volatile__(
        ".intel_syntax noprefix\n\t"
        "cmp dword ptr [edi+8], 1\n\t"
        "mov ecx, esi\n\t"
        "jne 1f\n\t"
        "mov eax, dword ptr [_ptr_dword_AB095C]\n\t"
        "mov eax, [eax]\n\t"
        "push eax\n\t"
        "push 0x59\n\t"
        "call dword ptr [_sub_713AA0]\n\t"
        "jmp 2f\n\t"
        "1:\n\t"
        "mov eax, dword ptr [_ptr_dword_AB0914]\n\t"
        "mov eax, [eax]\n\t"
        "push eax\n\t"
        "push 0x59\n\t"
        "call dword ptr [_sub_713AA0]\n\t"
        "mov eax, [esi+0x1330]\n\t"
        "test eax, eax\n\t"
        "je 2f\n\t"
        "push 0\n\t"
        "push 0x80\n\t"
        "mov ecx, esi\n\t"
        "call dword ptr [_sub_713A30]\n\t"
        "2:\n\t"
        "jmp dword ptr [_LightingFixUpdateMirrorCave_Exit]\n\t"
        ".att_syntax prefix\n\t"
    );
}

namespace
{

class LightingFixMirror
{
public:
    LightingFixMirror()
    {
        WFP::onInitEventAsync() += []()
        {
            CIniReader iniReader("");
            bool bLightingFix = iniReader.ReadInteger("GRAPHICS", "LightingFix", 1) != 0;

            if (bLightingFix)
            {
                uint32_t* dword_7497B9 = hook::pattern("8B 0D ? ? ? ? 83 F8 06 89 0D ? ? ? ? C7 05 ? ? ? ? CD CC CC 3E").count(2).get(1).get<uint32_t>(2);
                static float f20 = 2.0f;
                injector::WriteMemory((uint32_t)dword_7497B9, &f20, true);
                static float f19 = 1.9f;
                uint32_t* dword_748A70 = hook::pattern("A1 ? ? ? ? A3 ? ? ? ? 83 3D ? ? ? ? 06 C7 05 ? ? ? ? CD CC CC 3E").count(2).get(1).get<uint32_t>(1);
                injector::WriteMemory(dword_748A70, &f19, true);
                uint32_t* dword_73E7CB = *hook::pattern("C7 05 ? ? ? ? CD CC CC 3E B8 ? ? ? ? 74 05 B8 ? ? ? ? C3").count(1).get(0).get<uint32_t*>(11);
                injector::WriteMemory<float>(dword_73E7CB, 1.35f, true);

                //Lighting Fix Update (mirror)
                auto pattern = hook::pattern("8B 4F 6C 8B 91 8C 02 00 00 52 6A 6C 8B CE"); //0x00748C17
                uint32_t* dword_748C2A = pattern.count(1).get(0).get<uint32_t>(0x13);
                ptr_dword_AB095C = *pattern.count(1).get(0).get<uint32_t*>(0x1C);
                ptr_dword_AB0914 = *hook::pattern("52 50 FF 51 5C A1 ? ? ? ? 8B 08").count(1).get(0).get<uint32_t*>(6);
                sub_713AA0 = hook::pattern("C2 08 00 CC 8B 54 24 08 8B 41 44 56").count(1).get(0).get<uint32_t>(4);
                sub_713A30 = hook::pattern("89 81 84 17 00 00 C3 CC CC").count(1).get(0).get<uint32_t>(9);
                LightingFixUpdateMirrorCave_Exit = hook::pattern("A1 ? ? ? ? 8B 15 ? ? ? ? 83 F8 06 89 15 ? ? ? ? C7 05 ? ? ? ? CD CC CC 3E").count(1).get(0).get<uint32_t>(0); //0x00748C5D
                injector::MakeJMP(dword_748C2A, LightingFixUpdateMirrorCave, true);

                uint32_t* dword_72E382 = hook::pattern("C7 05 ? ? ? ? 01 00 00 00 C7 05 ? ? ? ? 00 00 80").count(1).get(0).get<uint32_t>(6);
                injector::WriteMemory(dword_72E382, 0, true);
            }
        };
    }
} LightingFixMirror;

} // anonymous namespace
