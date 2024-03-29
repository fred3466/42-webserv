#include "ResponseConfig.h"

ResponseConfig::ResponseConfig() : kv() {}

ResponseConfig::~ResponseConfig() {}

// Mimics Config::getParamInt
int ResponseConfig::getParamInt(std::string param, int intDefault)
{
    try
    {
        std::string res = kv[param];
        if (!res.empty())
        {
            std::stringstream ss(res);
            int resInt;
            ss >> resInt;
            return resInt;
        }
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
    return intDefault;
}

std::string ResponseConfig::getParamStr(std::string param, std::string stringDefault)
{
    try
    {
        std::string res = kv[param];
        if (!res.empty())
        {
            return res;
        }
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
    return stringDefault;
}

void ResponseConfig::addParam(std::string param, std::string value)
{
    kv[param] = value;
}
