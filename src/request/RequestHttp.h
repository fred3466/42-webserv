#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include <map>

class RequestHttp: public Request
{
private:
	std::string uri, method;
	RequestHeader *header;
	std::map<std::string, std::string> kv;
	int fdClient;

public:
	RequestHttp();
	~RequestHttp();
	RequestHttp(RequestHeader *header);

<<<<<<< HEAD
	virtual const std::list<std::string> getFields() const;
	virtual std::string getUri();
	virtual std::string getMethod();
	virtual void dump();
	virtual void setFdClient(int fd);
	virtual int getFdClient();
=======
	std::string getValue(std::string paramName) const;
	void addParam(std::string paramName, std::string paramValue);
	std::string getUri() const;
	std::string getMethod() const;
	void dump() const;
	void setFdClient(int fd);
	int getFdClient() const;
>>>>>>> branch 'main' of git@github.com:fred3466/42-webserv.git
};
