#include "RequestHttpBody.h"

RequestHttpBody::RequestHttpBody(std::string *rawRequest)
{
	if (!rawRequest)
	{
		content = new std::string("");
		return;
	}

	std::string tempContent = "";
	std::stringstream lines;
	lines.str(rawRequest->c_str());
	char line[2048];
	bool bBodyMode = false;

	while (lines)
	{
		lines.getline(line, 2048, '\n');
		std::stringstream lineSs;
		lineSs.str(line);
		std::string lineStr = lineSs.str();
		if (bBodyMode)
		{
			lineStr += "\n";
			tempContent += lineStr;
		}
		if (lineStr == "\r" || lineStr == "")
		{
			bBodyMode = true;
			continue;
		}

	}
	content = new std::string(tempContent);
}

std::string* RequestHttpBody::getContent()
{
	return content;
}

RequestHttpBody::~RequestHttpBody()
{
}

RequestHttpBody::RequestHttpBody(const RequestHttpBody &o) : RequestBody(), content(o.content)
{
	if (this != &o)
		*this = o;
}

RequestHttpBody& RequestHttpBody::operator=(const RequestHttpBody &o)
{
	this->content = o.content;
	return *this;
}

