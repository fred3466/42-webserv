#pragma once
#include <string>
#include <map>
#include <string>
#include <iostream>
#include "../API/RequestHeader.h"
#include <sstream>

class Request
{
private:
public:
	Request();
	Request(RequestHeader *header);
	virtual ~Request();

	virtual const std::list<std::string> getFields() const =0;
	virtual std::string getUri()=0;
	virtual std::string getMethod()=0;
	virtual void dump()=0;
	virtual void setFdClient(int fd)=0;
	virtual int getFdClient()=0;
};

