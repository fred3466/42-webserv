#pragma once
#include <string>

#include "../API/ResponseHeader.h"

class Response
{
public:
	Response();
	virtual ~Response();

	virtual void setBody(std::string sline)=0;
	virtual void setBodyBin(char *bytess)=0;

//	Response(const Response &o);
//	Response& operator=(const Response &o);

	virtual ResponseHeader* getHeader()=0;
	virtual std::string getBody()=0;
	virtual char* getBodyBin()=0;
	virtual int getBodyLength()=0;
	virtual void setBodyLength(int len)=0;

};

