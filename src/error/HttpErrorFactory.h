#pragma once
#include "../response/API/Response.h"
#include "../response/ResponseHttpHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/ResponseHttp.h"
#include <sstream>
#include <string>
#include <vector>

class HttpErrorFactory
{
private:
    static std::vector<HttpError> errorList;
    static void loadErrorList();

public:
    static void initialize();
    static HttpError *build(int errorCode);
};

// class HttpErrorFactory
// {
// private:
// public:
//     static HttpError *build(int errorCode);
//     static Response *generateErrorResponse(int errorCode, const std::string &errorMessage);
// };
