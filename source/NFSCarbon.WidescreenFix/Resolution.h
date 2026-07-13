#pragma once

#include <utility>

// Public interface formerly exposed via `export module Resolution;`.
// Everything else that used to live in this module (ResList, DALOptions,
// the WndProc hook, the Resolution registration class, etc.) is
// implementation detail and stays inside Resolution.cpp.

std::pair<int, int> GetRes();
float GetAspectRatio();
