#include <stdafx.h>
#include "common.h"

namespace
{

class ConsoleGamma
{
public:
    ConsoleGamma()
    {
        CIniReader iniReader("");
        auto bConsoleGamma = iniReader.ReadInteger("GRAPHICS", "ConsoleGamma", 0) != 0;
        (void)bConsoleGamma;
    }
} ConsoleGamma;

}
