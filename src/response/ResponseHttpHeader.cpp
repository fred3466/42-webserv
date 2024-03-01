#include "ResponseHttpHeader.h"

ResponseHttpHeader::ResponseHttpHeader() :
		fields()
{
}

ResponseHttpHeader::~ResponseHttpHeader()
{
}

void ResponseHttpHeader::addField(std::string f)
{
	if (!f.empty())
		fields.push_back(f);
}
const std::list<std::string>& ResponseHttpHeader::getFields() const
{
	return fields;
}

const std::string& ResponseHttpHeader::getStatusLine() const
{
	return statusLine;
}

void ResponseHttpHeader::setStatusLine(const std::string &statusLine)
{
	this->statusLine = statusLine;
}
