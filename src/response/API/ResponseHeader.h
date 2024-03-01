#pragma once
#include <string>
#include <list>

class ResponseHeader
{
private:

public:
	ResponseHeader();
	virtual ~ResponseHeader();

	virtual void addField(std::string f)=0;
	virtual const std::list<std::string>& getFields() const =0;
	virtual const std::string& getStatusLine() const =0;
	virtual void setStatusLine(const std::string &statusLine)=0;
};

