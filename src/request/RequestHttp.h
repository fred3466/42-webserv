#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include "../util/StringUtil.h"
#include <map>
#include <iterator>
#include <string>

class RequestHttp : public Request
{
private:
	std::string uri, method, body;
	RequestHeader *header;
	int fdClient;

public:
	RequestHttp();
	~RequestHttp();
	RequestHttp(RequestHeader *header);

	virtual const std::list<std::string> &getFields() const;
	virtual std::string getHeaderFieldValue(std::string fieldName) const;
	virtual void addField(std::string rawField) const;
	virtual std::string getUri() const;
	virtual std::string getMethod() const;
	//	virtual void dump() const;
	virtual void setFdClient(int fd);
	virtual int getFdClient() const;
	virtual std::string getQueryString() const;
	std::string getPath() const;
	std::string getHost() const;
	virtual RequestHeader *getHeader() const;
	virtual std::string getFileExtension() const;

	void setBody(const std::string &b);
	const std::string &getBody() const;
};
