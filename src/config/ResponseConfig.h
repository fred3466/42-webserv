#pragma once
#include <string>
#include <map>
#include <sstream>
#include <iostream>

class ResponseConfig
{
private:
    std::map<std::string, std::string> kv;

public:
    ResponseConfig();
    ~ResponseConfig();

    int getParamInt(std::string param, int intDefault);
    std::string getParamStr(std::string param, std::string stringDefault);
    void addParam(std::string param, std::string value);
};
