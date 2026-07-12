#pragma once

#include <utility>

// Public interface formerly exposed via `export module Resolution;`.
// Everything else that used to live in this module (ResList, WndProc hook,
// the resolution config load/save helpers, the Resolution registration
// class, etc.) is implementation detail and stays inside Resolution.cpp.

void SetRes(int width, int height);
std::pair<int, int> GetRes();
float GetAspectRatio();
