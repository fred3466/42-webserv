#pragma once
#include "../request/Request.h"
#include <map>
#include <string>
#include <iostream>
#include <sstream>

class RequestHttp: public Request
{
private:
	std::string uri, method;
	std::map<std::string, std::string> kv;
	int fdClient;
public:
	RequestHttp();
	~RequestHttp();
//	RequestHttp(const RequestHttp &other);
//	RequestHttp& operator=(const RequestHttp &other);
	RequestHttp(std::string *rawContent);

	std::string getValue(std::string paramName);
	void addParam(std::string paramName, std::string paramValue);
	std::string getUri();
	std::string getMethod();
	void dump();
	void setFdClient(int fd);
	int getFdClient();
};

