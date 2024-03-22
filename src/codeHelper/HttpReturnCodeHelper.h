#pragma once
#include "../response/ResponseTools.h"
#include <sstream>
#include <string>

class HttReturnCodeHelper
{
public:
    HttReturnCodeHelper();
    std::string getStatusLine(int httpReturnCode);
};
