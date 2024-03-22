#pragma once
#include <string>

#include "../API/ResponseHeader.h"
//#include "../../error/HttpError.h"

class Response
{
public:
	Response();
	virtual ~Response();

//	virtual void setBody(std::string sline)=0;

//	Response(const Response &o);
//	Response& operator=(const Response &o);

	virtual ResponseHeader* getHeader()=0;
	//	virtual std::string getBody()=0;
	virtual char* getBodyBin()=0;
	virtual void setBodyBin(char *bytess)=0;
	virtual int getBodyLength()=0;
	virtual void setBodyLength(int len)=0;
	virtual int getTotalLength()=0;
	virtual void setTotalLength(int len)=0;
	virtual bool isCgi()=0;
	virtual bool isError()=0;
	//	virtual HttpError* getError()=0;

	virtual void setCgi(bool cgi)=0;
	//	virtual void setError(HttpError *error)=0;
	//	TODO @Anastasia : temporaire, à virer après intégration de HttpError
	virtual void setIsError(bool isError)=0;
};

