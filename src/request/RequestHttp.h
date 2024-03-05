#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include <map>

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
	RequestHttp(RequestHeader *header);

	std::string getValue(std::string paramName);
	void addParam(std::string paramName, std::string paramValue);
	std::string getUri();
	std::string getMethod();
	void dump();
	void setFdClient(int fd);
	int getFdClient();
};

