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
	virtual std::list<std::string> getFields() =0;
	virtual std::string getStatusLine() =0;
	virtual void setStatusLine(std::string statusLine)=0;
};

