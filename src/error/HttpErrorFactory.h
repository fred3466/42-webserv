#pragma once
#include "../response/ResponseHttpHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/ResponseHttp.h"
#include <sstream>
#include <string>
#include <vector>
#include "../API/Response.h"
#include "../config/Config.h"

class HttpErrorFactory
{
private:
	std::vector<HttpError> errorList;
	void loadErrorList();
	//	Config *config;

public:
	HttpErrorFactory();
	~HttpErrorFactory();
	//	void initialize(Config *config);
	HttpError* build(int errorCode);
};

// class HttpErrorFactory
// {
// private:
// public:
//     static HttpError *build(int errorCode);
//     static Response *generateErrorResponse(int errorCode, const std::string &errorMessage);
// };
