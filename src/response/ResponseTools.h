#pragma once
#include <string>
#include <map>

class ResponseTools
{
private:
	std::map<int, std::string> codeReasonMap;

public:
	ResponseTools();
	~ResponseTools();

	std::string buildStatusLineForCode(int code);
};

