#pragma once
#include <string>
#include <list>

class RequestHeader
{
private:

public:
	RequestHeader();
	virtual ~RequestHeader();

	virtual void addField(std::string f)=0;
	virtual const std::list<std::string>& getFields() const =0;
	virtual const std::string& getMethod() const =0;
	virtual void setMethod(const std::string &method)=0;
	virtual const std::string& getUri() const =0;
	virtual void setUri(const std::string &uri)=0;
	virtual const std::string& getVersion() const =0;
	virtual void setVersion(const std::string &version)=0;
};