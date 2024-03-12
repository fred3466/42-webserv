#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include <map>
#include <iterator>
#include <string>

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

	virtual const std::list<std::string>& getFields() const;
	virtual std::string getUri() const;
	virtual std::string getMethod() const;
	virtual void dump() const;
	virtual void setFdClient(int fd);
	virtual int getFdClient() const;
};
