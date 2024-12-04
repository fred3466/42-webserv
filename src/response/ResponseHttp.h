#pragma once
#include "../API/Response.h"
#include "../API/ResponseHeader.h"
#include "../error/HttpError.h"

class HttpError;
class ResponseHttp: public Response {
private:
	ResponseHeader *header;
	char *bodyBin;
	int bodyLength;
	int totalLength;
	bool flagCgi;
	bool flagRedirect;
	int errorCodeTmp;
	bool bNeedFurtherProcessing;
	int fd;

public:
	ResponseHttp(ResponseHeader *header);
	~ResponseHttp();

	void setStatusLine(std::string sline);
	void setHeader(std::string sline);
	void setBodyBin(char *bytess);
	std::string getStatusLine();
	char* getBodyBin();
	int getBodyLength();
	void setBodyLength(int len);
	ResponseHeader* getHeader();
	virtual int getTotalLength();
	virtual void setTotalLength(int len);
	virtual bool isCgi();
	virtual bool isRedirect();
	virtual void setCgi(bool cgi);
	virtual void setIsRedirect(bool isRedirect);
	virtual int getErrorCodeTmp();
	virtual void setErrorCodeTmp(int errorCode);
	virtual bool isBNeedFurtherProcessing();
	virtual void setBNeedFurtherProcessing(bool bNeedFurtherProcessing);
	virtual int getFd();
	virtual void setFd(int fd);
};
