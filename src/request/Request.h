#pragma once
#include <string>
#include <map>
#include <string>
#include <iostream>
#include <sstream>

class Request
{
private:
public:
	Request();
	virtual ~Request();
	Request(std::string *rawContent);
	//	Request(Request const &o);
	//	Request& operator=(Request const &o);

	virtual std::string getValue(std::string paramName) const = 0;
	virtual void addParam(std::string paramName, std::string paramValue) = 0;
	virtual std::string getUri() const = 0;
	virtual std::string getMethod() const = 0;
	virtual void dump() const = 0;
	virtual void setFdClient(int fd) = 0;
	virtual int getFdClient() = 0;
};
