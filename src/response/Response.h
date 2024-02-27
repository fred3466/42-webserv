#pragma once
#include <string>
class Response
{
public:
	Response();
	~Response();
	virtual void setStatusLine(std::string sline)=0;
	virtual void setHeader(std::string sline)=0;
	virtual void setBody(std::string sline)=0;

//	Response(const Response &o);
//	Response& operator=(const Response &o);

	virtual std::string getStatusLine()=0;
	virtual std::string getHeader()=0;
	virtual std::string getBody()=0;
	virtual int getBodyLength()=0;
	virtual void setBodyLength(int len)=0;

};

