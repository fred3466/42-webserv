#pragma once

#include <string>
#include <cstring>
#include "../util/StringUtil.h"
#include "../util/FileUtil.h"
#include "../API/RequestHeader.h"
#include "factory/RequestHeaderFactory.h"
#include "factory/RequestBodyFactory.h"
#include "../Harl.h"

class RequestHelper
{
private:
	RequestHelper();
	Harl harl;
	char *rawRequestBuffer;
	int rawRequestBuffer_Length;
	size_t requestBodyLen;
	size_t bodyContent_Length;
	bool bFragmentedMode;
	bool bJustInitialized;

//	std::string rawRequest;
//	int rawRequestLen;

public:
	RequestHelper(char *const rawRequest, int const rawRequestLen);
	~RequestHelper();

	char* process(char *rawRequest, int rawRequestLen);
	int getRawRequestBufferLength() const;
	void setRawRequestBufferLength(int rawRequestBufferLength);
};

