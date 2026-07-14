<div align="center">

![wfp](https://raw.githubusercontent.com/ThirteenAG/thirteenag.github.io/master/img/logo2-bl.svg)
# ... but MinGW (compile on Linux!)
[Because MSVC is for losers.](https://youtu.be/iowq8OXyj-U?t=140) Also, we support WinXP.

[![Downloads](https://img.shields.io/github/downloads/Fierelier/WidescreenFixesPack/total?color=red)](https://github.com/Fierelier/WidescreenFixesPack/releases/)

Plugins ~~to make or improve widescreen resolutions support in PC games, add more features and fix bugs.~~ that do way too much.
</div>

# Differences to main repo

- **Unavailable functionality:**
  - Not all games are supported
  - Shader-related things like ConsoleGamma
  - DX9 paths for GTA III/VC TransparentMenu

- **Quirks:**
  - Bigger .asi files, not sure why
  - Probably more..?

- **Fixes:**
  - Windows XP support (the main reason for this fork existing)
  - Dynamic menu scissoring in NFS Underground, pillarboxing crash
  - NFS ProStreet FMV scaling on non-16:9 monitors
  - Some games no longer use graphically taxing defaults

# Building

You can find the resulting files in `data/*.WidescreenFix/scripts`.

The following SUITEs are supported:

- NFSUnderground.WidescreenFix
- NFSUnderground2.WidescreenFix
- NFSMostWanted.WidescreenFix
- NFSCarbon.WidescreenFix
- NFSProStreet.FusionFix
- GTA3.WidescreenFix

## Premake 5

Requirements:

- [Premake 5](https://premake.github.io/) _(pre-built executable available in this repository root)_
- sh
- make
- MinGW

```sh
wine premake5.exe gmake
./patch-makefiles.sh
cd build
SUITE=NFSUnderground.WidescreenFix
export AR=i686-w64-mingw32-ar
export CC=i686-w64-mingw32-gcc
export CXX=i686-w64-mingw32-g++
export RESCOMP=i686-w64-mingw32-windres
make -j$(nproc) -f $SUITE.make # add config=debug_win32 for debug
```

## sh

I recommend this path on Windows. You can simply grab [busybox-w32](https://frippery.org/busybox/) and [mingw-lite](https://github.com/redpanda-cpp/mingw-lite) to get started. This method even allows you to compile right from Windows XP.

Requirements:

- sh
- MinGW

```sh
cd build_alt
SUITE=NFSUnderground.WidescreenFix
./$SUITE.sh # or debug_$SUITE.sh, for debug
```

# XP Support

To enable XP support, edit `include/targetver.h` and change `#define WINVER 0x0600` to `#define WINVER 0x0501`. Note that this requires a MinGW with POSIX threads, I believe. If you want to support CPUs without SSE(2), you should also use a MinGW that isn't compiled with it. You can use a Gentoo i486 chroot for example, or [mingw-lite](https://github.com/redpanda-cpp/mingw-lite) (one of the i486 w98 builds) on Windows.

# How to Report Crashes

Open an issue. Try to send some sort of useful crashdump, preferably use a debug .asi.

# Trash
- How to search for evil: `grep -rnE '\b(fopen_s|freopen_s|tmpfile_s|tmpnam_s|fprintf_s|fscanf_s|printf_s|scanf_s|snprintf_s|sprintf_s|sscanf_s|vfprintf_s|vfscanf_s|vprintf_s|vscanf_s|vsnprintf_s|vsprintf_s|vsscanf_s|gets_s|strcpy_s|strncpy_s|strcat_s|strncat_s|strtok_s|strerror_s|strerrorlen_s|memcpy_s|memmove_s|memset_s|wcscpy_s|wcsncpy_s|wcscat_s|wcsncat_s|wcstok_s|wmemcpy_s|wmemmove_s|wcrtomb_s|mbstowcs_s|wcstombs_s|mbsrtowcs_s|wcsrtombs_s|swprintf_s|vswprintf_s|fwprintf_s|fwscanf_s|swscanf_s|vfwprintf_s|vfwscanf_s|vwprintf_s|vwscanf_s|wprintf_s|wscanf_s|bsearch_s|qsort_s|wctomb_s|getenv_s|asctime_s|ctime_s|localtime_s|gmtime_s)\s*\(' .`
