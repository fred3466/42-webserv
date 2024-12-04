#pragma once
#include <string>

#include "ResponseHeader.h"
#include "../FdPollfdServerTuple.h"

class Response {
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
	virtual void setErrorCodeTmp(int errorCode) = 0;
	virtual int getErrorCodeTmp() = 0;
	virtual void setStatusLine(std::string sline) = 0;
	virtual bool isBNeedFurtherProcessing() = 0;
	virtual void setBNeedFurtherProcessing(bool bNeedFurtherProcessing) = 0;
	virtual int getFd()=0;
	virtual void setFd(int fd)=0;

};
