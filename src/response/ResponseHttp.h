#pragma once
#include "API/ResponseHeader.h"
#include "API/Response.h"
//#include "../error/HttpError.h"

//class HttpError;
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
	//	HttpError *error;

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

};

