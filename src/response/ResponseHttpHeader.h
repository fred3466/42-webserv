#pragma once
#include <string>
#include <list>
#include "API/ResponseHeader.h"

class ResponseHttpHeader: public ResponseHeader
{
private:
	std::string statusLine;
	std::list<std::string> fields;

public:
	ResponseHttpHeader();
	virtual ~ResponseHttpHeader();

	virtual void addField(std::string f);
	virtual std::list<std::string> getFields();
	virtual std::string getStatusLine();
	virtual void setStatusLine(std::string statusLine);
};

