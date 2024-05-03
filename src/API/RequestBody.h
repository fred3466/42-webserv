#pragma once
#include <string>

class RequestBody
{
public:
	RequestBody(char *rawRequest, int rawRequestLen);
	RequestBody();
	virtual ~RequestBody();
	RequestBody(const RequestBody &other);
	RequestBody& operator=(const RequestBody &other);
	virtual char* getContent()=0;
	virtual int getLen()=0;
};

