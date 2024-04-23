#pragma once
#include "../response/ResponseTools.h"
#include "../util/StringUtil.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>

class HttpReturnCodeHelper
{
private:
	HttpReturnCodeHelper();

	int httpReturnCode;

public:
	HttpReturnCodeHelper(int httpReturnCode);
	~HttpReturnCodeHelper();
	//	static std::string getStatusLine();
	void replacePlaceholders(std::string &content, const std::string &errorMessage);
};
