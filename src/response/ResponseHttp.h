#pragma once
#include "API/ResponseHeader.h"
#include "API/Response.h"

class ResponseHttp: public Response
{
private:
	std::string status_line;
	ResponseHeader *header;
	std::string body;
	char *bodyBin;
	int bodyLength;

public:
	ResponseHttp(ResponseHeader *header);
	~ResponseHttp();
	//	ResponseHttp(const ResponseHttp &other);
//	ResponseHttp& operator=(const ResponseHttp &other);

	void setStatusLine(std::string sline);
	void setHeader(std::string sline);
	void setBody(std::string sline);
	void setBodyBin(char *bytess);
	std::string getStatusLine();
	std::string getBody();
	char* getBodyBin();
	int getBodyLength();
	void setBodyLength(int len);
	ResponseHeader* getHeader();
};

