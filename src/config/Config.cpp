#include "Config.h"

#include <iostream>
#include <fstream>
#include <sstream>

Config::Config() :
		kv()
{

}

Config::~Config()
{
	// TODO Auto-generated destructor stub
}

//Config::Config(const Config &other)
//{
//	// TODO Auto-generated constructor stub
//
//}
//
//Config& Config::operator=(const Config &other)
//{
//	// TODO Auto-generated method stub
//
//}

void Config::read(std::string path)
{
	std::string key, val;

	std::ifstream is(path.c_str());

	std::string line;
	if (is.is_open())
	{
		while (std::getline(is, line))
		{
			std::stringstream ss(line);
			std::getline(ss, key, '=');
//			key << ss;
			std::getline(ss, val, '=');
			kv[key] = val;
			std::cout << key << " -> " << val << std::endl;
		}
		is.close();
	}
}

std::string Config::getParamStr(std::string param)
{
	try
	{
//		if (kv.count(param) > 0)
//		{
//		std::map<std::string, std::string>::iterator it;
//		it = kv.find(param);
		std::string res = kv[param];
		if (!res.empty())
		{
			return res;
		}
	} catch (const std::out_of_range &oor)
	{
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		return "";
	}
	return "";
}
