#pragma once
#include "../util/StringUtil.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>

class HttpReturnCodeHelper {
private:
	HttpReturnCodeHelper();

	int httpReturnCode;

public:
	HttpReturnCodeHelper(int httpReturnCode);
	~HttpReturnCodeHelper();
	void replacePlaceholders(std::string &content, const std::string &errorMessage);
};
