#include <stdafx.h>
#include "common.h"

#include "CutsceneMgr.h"

CCutsceneMgr::CCutsceneMgr()
{
    WFP::onGameInitEvent() += []()
    {
        auto pattern = hook::pattern("88 1D ? ? ? ? 88 1D ? ? ? ? 88 1D ? ? ? ? 88 1D ? ? ? ? 88 1D ? ? ? ? 88 1D ? ? ? ? E8 ? ? ? ? 83 C4");
        ms_running.SetAddress(*pattern.get_first<bool*>(2));

        pattern = hook::pattern("88 1D ? ? ? ? 88 1D ? ? ? ? 88 1D ? ? ? ? E8 ? ? ? ? 83 C4");
        ms_useLodMultiplier.SetAddress(*pattern.get_first<bool*>(2));
    };
}

namespace
{

CCutsceneMgr CutsceneMgr;

}
