#pragma once

#include "API/Request.h"
#include "API/RequestHeader.h"
#include <map>

class RequestHttpHeader: public RequestHeader
{
private:
	std::string statusLine;
	std::list<std::string> fields;
	std::string method;
	std::string uri;
	std::string version;

public:
	RequestHttpHeader(std::string *rawRequest);
	virtual ~RequestHttpHeader();

	virtual void addField(std::string f);
	virtual const std::list<std::string>& getFields() const;
	virtual const std::string& getMethod() const;
	virtual void setMethod(const std::string &method);
	virtual const std::string& getUri() const;
	virtual void setUri(const std::string &uri);
	virtual const std::string& getVersion() const;
	virtual void setVersion(const std::string &version);
};