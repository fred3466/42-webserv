#include "RequestConfig.h"

RequestConfig::RequestConfig() : kv() {}

RequestConfig::~RequestConfig() {}

int RequestConfig::getParamInt(std::string param, int intDefault)
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

std::string RequestConfig::getParamStr(std::string param, std::string stringDefault)
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

void RequestConfig::addParam(std::string param, std::string value)
{
    kv[param] = value;
}
