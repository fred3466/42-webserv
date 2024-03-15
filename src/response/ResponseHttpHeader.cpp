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
std::list<std::string> ResponseHttpHeader::getFields()
{
	return fields;
}

std::string ResponseHttpHeader::getStatusLine()
{
	return statusLine;
}

void ResponseHttpHeader::setStatusLine(std::string statusLine)
{
	this->statusLine = statusLine;
}
