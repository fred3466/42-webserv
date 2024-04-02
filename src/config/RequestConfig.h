#pragma once
#include <string>
#include <map>
#include <sstream>
#include <iostream>

class RequestConfig
{
private:
    std::map<std::string, std::string> kv;

public:
    RequestConfig();
    ~RequestConfig();

    int getParamInt(std::string param, int intDefault);
    std::string getParamStr(std::string param, std::string stringDefault);
    void addParam(std::string param, std::string value);
};
