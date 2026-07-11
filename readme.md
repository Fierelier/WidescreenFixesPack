<div align="center">

[![wfp](https://raw.githubusercontent.com/ThirteenAG/thirteenag.github.io/master/img/logo2-bl.svg)](https://fusionfix.io/wfp)
# ... but MinGW (compile on Linux!)
[Because MSVC is for losers.](https://youtu.be/iowq8OXyj-U?t=140)

[![Downloads](https://img.shields.io/github/downloads/Fierelier/WidescreenFixesPack/total?color=red)](https://github.com/Fierelier/WidescreenFixesPack/releases/)

Plugins ~~to make or improve widescreen resolutions support in PC games, add more features and fix bugs.~~ that do way too much.
</div>

# Building and Installing

**Currently, only NFSMostWanted is supported**

Requirements:

- [Premake 5](https://premake.github.io/) _(pre-built executable available in this repository root)_
- ~~Visual Studio~~ (awful)
- MinGW (baaazzeeddd!!!)

```sh
wine premake5.exe gmake
./patch-makefiles.sh
cd build
make -j$(nproc) -f NFSMostWanted.WidescreenFix.make
```

# How to Report Crashes

Open an issue. Don't bother sending crash dumps, I can't read that shit.

# Important notes

- Not compatible with Windows XP (without recompiling with `_xp` toolset and `/Zc:threadSafeInit-`). -- **CHALLENGE ACCEPTED!!**
