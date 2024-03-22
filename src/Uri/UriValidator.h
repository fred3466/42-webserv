#pragma once

#include <iostream>
#include <vector>
#include <sstream>

class UriValidator
{
public:
	bool checkScheme(const std::string &scheme);
	std::string formatPath(const std::string &path);
};