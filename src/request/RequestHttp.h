#pragma once

#include "Request.h"
#include <map>

class RequestHttp : public Request
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

	std::string getValue(std::string paramName) const;
	void addParam(std::string paramName, std::string paramValue);
	std::string getUri() const;
	std::string getMethod() const;
	void dump() const;
	void setFdClient(int fd);
	int getFdClient();
};
