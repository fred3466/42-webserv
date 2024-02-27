#pragma once
#include <string>
#include <map>
#include <stdexcept>

class Config
{
private:
	std::map<std::string, std::string> kv;
public:
	Config();
	~Config();
//	Config(const Config &other);
//	Config& operator=(const Config &other);
	void read(std::string path);
	std::string getParamStr(std::string param);
};

