#pragma once
#include "string"

class Request
{
private:

public:
//	Request();
	virtual ~Request();
//	Request(Request const &o);
//	Request& operator=(Request const &o);

	virtual std::string getValue(std::string paramName)=0;
	virtual void addParam(std::string paramName, std::string paramValue)=0;
};

