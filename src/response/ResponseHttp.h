#pragma once
#include "Response.h"

class ResponseHttp: public Response
{
private:
	std::string status_line, header;
	std::string body;
	int bodyLength;
public:
	ResponseHttp();
	~ResponseHttp();
//	ResponseHttp(const ResponseHttp &other);
//	ResponseHttp& operator=(const ResponseHttp &other);

	void setStatusLine(std::string sline);
	void setHeader(std::string sline);
	void setBody(std::string sline);
	std::string getStatusLine();
	std::string getHeader();
	std::string getBody();
	int getBodyLength();
	void setBodyLength(int len);
};

