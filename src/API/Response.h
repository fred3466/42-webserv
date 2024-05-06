#pragma once
#include <string>

#include "../API/ResponseHeader.h"
#include "../error/HttpError.h"
// #include "../../error/HttpErrorFactory.h"

class Response
{
public:
	Response();
	virtual ~Response();
	virtual ResponseHeader* getHeader() = 0;
	virtual char* getBodyBin() = 0;
	virtual void setBodyBin(char *bytess) = 0;
	virtual int getBodyLength() = 0;
	virtual void setBodyLength(int len) = 0;
	virtual int getTotalLength() = 0;
	virtual void setTotalLength(int len) = 0;
	virtual bool isCgi() = 0;
	virtual bool isRedirect() = 0;
	virtual void setCgi(bool cgi) = 0;
	virtual void setIsRedirect(bool isRedirect) = 0;
	virtual void setHttpError(HttpError *error) = 0;
	virtual HttpError* getHttpError() = 0;
	virtual void setErrorCodeTmp(int errorCode) = 0;
	virtual int getErrorCodeTmp() = 0;
	virtual void setStatusLine(std::string sline) = 0;
};
