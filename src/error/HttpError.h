#pragma once
#include <iostream>
#include "../response/ResponseHttp.h"
#include "../response/ResponseHttpHeader.h"
class HttpError
{
private:
	int code;
	std::string description;

public:
	HttpError(/* args */);
	~HttpError();

	ResponseHttp* generateErrorResponse(int errorCode, const std::string &errorMessage);
	std::string loadErrorPageTemplate();
	void replacePlaceholders(std::string &content, int errorCode, const std::string &errorMessage);
};
