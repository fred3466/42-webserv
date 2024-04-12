#pragma once
#include "../API/Response.h"
#include "../API/ResponseHeader.h"
#include "../error/HttpError.h"

class HttpError;
class ResponseHttp: public Response
{
private:
	ResponseHeader *header;
	std::string body;
	char *bodyBin;
	int bodyLength;
	int totalLength;
	bool flagCgi;
	bool flagError;
	HttpError *error;
	int errorCodeTmp;

public:
	ResponseHttp(ResponseHeader *header);
	~ResponseHttp();
	//	ResponseHttp(const ResponseHttp &other);
	//	ResponseHttp& operator=(const ResponseHttp &other);

	void setStatusLine(std::string sline);
	void setHeader(std::string sline);
	//	std::string getBody();
	//	void setBody(std::string sline);
	void setBodyBin(char *bytess);
	std::string getStatusLine();
	char* getBodyBin();
	int getBodyLength();
	void setBodyLength(int len);
	ResponseHeader* getHeader();
	virtual int getTotalLength();
	virtual void setTotalLength(int len);
	virtual bool isCgi();
	virtual bool isError();
	//	virtual HttpError* getError();

	virtual void setCgi(bool cgi);
	//	virtual void setError(HttpError *error);
	//	TODO @Anastasia : temporaire, à virer après intégration de HttpError
	virtual void setIsError(bool isError);
	virtual void setHttpError(HttpError *error);
	virtual int getErrorCodeTmp();
	virtual void setErrorCodeTmp(int errorCode);
	virtual HttpError* getHttpError();
};
