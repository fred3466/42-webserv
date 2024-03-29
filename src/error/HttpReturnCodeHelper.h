#pragma once
#include "../response/ResponseTools.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>

class HttReturnCodeHelper
{
public:
    HttReturnCodeHelper();
    std::string getStatusLine(int httpReturnCode);
    std::string loadErrorPageTemplate();
    // void replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage);
};
