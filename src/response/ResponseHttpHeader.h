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
	virtual const std::list<std::string>& getFields() const;
	virtual const std::string& getStatusLine() const;
	virtual void setStatusLine(const std::string &statusLine);
};

