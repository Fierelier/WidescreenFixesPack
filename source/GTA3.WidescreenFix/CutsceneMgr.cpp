#include <stdafx.h>
#include "common.h"

#include "CutsceneMgr.h"

CCutsceneMgr::CCutsceneMgr()
{
    WFP::onGameInitEvent() += []()
    {
        auto pattern = hook::pattern("80 3D ? ? ? ? ? 0F 84 ? ? ? ? D9 05 ? ? ? ? D8 0D");
        ms_running.SetAddress(*pattern.get_first<bool*>(2));

        pattern = hook::pattern("C6 05 ? ? ? ? ? E8 ? ? ? ? A1");
        ms_useLodMultiplier.SetAddress(*pattern.get_first<bool*>(2));
    };
}

namespace
{

CCutsceneMgr CutsceneMgr;

}
