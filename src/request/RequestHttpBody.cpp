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
	char line[2048000];
	bool bBodyMode = false;

	while (lines)
	{
		lines.getline(line, 2048, '\n');
		std::stringstream lineSs;
		lineSs.str(line);
		std::string lineStr = lineSs.str() + "\n";
		if (bBodyMode /* && lineStr != ""*/)
		{
//			lineStr += "\n";
			tempContent += lineStr;
		}
		if (lineStr == "\r\n" || lineStr == "")
		{
			bBodyMode = true;
			continue;
		}

	}
	content = new std::string(tempContent);

//	char *bodyBin = new char[errorPageContent.length() + 1];
//	std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBin);
//	bodyBin[errorPageContent.length()] = '\0'; // Null-terminate the string
//	response->setBodyBin(bodyBin);
}

std::string* RequestHttpBody::getContent()
{
	return content;
}

RequestHttpBody::~RequestHttpBody()
{
	delete content;
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

