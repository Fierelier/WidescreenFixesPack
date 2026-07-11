<div align="center">

![wfp](https://raw.githubusercontent.com/ThirteenAG/thirteenag.github.io/master/img/logo2-bl.svg)
# ... but MinGW (compile on Linux!)
[Because MSVC is for losers.](https://youtu.be/iowq8OXyj-U?t=140) Also, we support WinXP.

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
AR=i686-w64-mingw32-ar CC=i686-w64-mingw32-gcc CXX=i686-w64-mingw32-g++ RESCOMP=i686-w64-mingw32-windres make -j$(nproc) -f NFSMostWanted.WidescreenFix.make
```

You can find the resulting files in `data/NFSMostWanted.WidescreenFix/scripts`.

# XP Support

To enable XP support, edit `include/targetver.h` and change `#define _WIN32_WINNT 0x0600` to `#define _WIN32_WINNT 0x0500`. Note that this requires a MinGW with POSIX threads, I believe. If you want to support CPUs without SSE(2), you should also use a MinGW that isn't compiled with it. You can use a Gentoo i486 chroot for example, or [mingw-lite](https://github.com/redpanda-cpp/mingw-lite) (one of the i486 w98 builds) on Windows.

# How to Report Crashes

Open an issue. Don't bother sending crash dumps, I can't read that shit.

# Important notes

- ~~Not compatible with Windows XP (without recompiling with `_xp` toolset and `/Zc:threadSafeInit-`).~~ -- **liberated af.....**

# Trash
- How to search for evil: `grep -rnE '\b(fopen_s|freopen_s|tmpfile_s|tmpnam_s|fprintf_s|fscanf_s|printf_s|scanf_s|snprintf_s|sprintf_s|sscanf_s|vfprintf_s|vfscanf_s|vprintf_s|vscanf_s|vsnprintf_s|vsprintf_s|vsscanf_s|gets_s|strcpy_s|strncpy_s|strcat_s|strncat_s|strtok_s|strerror_s|strerrorlen_s|memcpy_s|memmove_s|memset_s|wcscpy_s|wcsncpy_s|wcscat_s|wcsncat_s|wcstok_s|wmemcpy_s|wmemmove_s|wcrtomb_s|mbstowcs_s|wcstombs_s|mbsrtowcs_s|wcsrtombs_s|swprintf_s|vswprintf_s|fwprintf_s|fwscanf_s|swscanf_s|vfwprintf_s|vfwscanf_s|vwprintf_s|vwscanf_s|wprintf_s|wscanf_s|bsearch_s|qsort_s|wctomb_s|getenv_s|asctime_s|ctime_s|localtime_s|gmtime_s)\s*\(' .`
