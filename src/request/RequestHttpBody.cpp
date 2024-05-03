#include "RequestHttpBody.h"

RequestHttpBody::RequestHttpBody(char *rawRequest, int rawRequestLen)
{
	if (!rawRequest)
	{
		content = new char[0];
		this->len = 0;
		return;
	}

//	char *tempContent;
	std::stringstream lines;
	lines.str(rawRequest);
	char line[4096];
	memset(line, 0, 4096);
	bool bBodyMode = false;
	int headerSize = 0;
	int szCopy = 0;

	while (lines)
	{
		lines.getline(line, 4096, '\n');
		std::string lineStr = std::string(line);
		if (!lines.eof() && !lines.fail())
			lineStr += "\n";
		headerSize += lineStr.length();
//		std::stringstream lineSs;
//		lineSs.str(line);
		if (!bBodyMode && (lineStr == "\r\n" || lineStr == ""))
		{
//			lineStr += "\n";
			szCopy = rawRequestLen - headerSize;
			content = new char[szCopy];
			memcpy(content, rawRequest + headerSize, szCopy);
			this->len = szCopy;
			break;
		}

	}

	if (HARL_LEVEL >= 3)
	{
		std::string fname = "DBG/_REQUEST_content.txt";
		std::ofstream os(fname.c_str(), std::ios::binary | std::ios::out);
		os.write(content, szCopy);
		os.close();
	}

//	char *bodyBin = new char[errorPageContent.length() + 1];
//	std::copy(errorPageContent.begin(), errorPageContent.end(), bodyBin);
//	bodyBin[errorPageContent.length()] = '\0'; // Null-terminate the string
//	response->setBodyBin(bodyBin);
}

int RequestHttpBody::getLen()
{
	return len;
}

void RequestHttpBody::setLen(int len)
{
	this->len = len;
}

char* RequestHttpBody::getContent()
{
	return content;
}

RequestHttpBody::~RequestHttpBody()
{
	delete content;
}

RequestHttpBody::RequestHttpBody(const RequestHttpBody &o) : RequestBody(), content(o.content), len(o.len)
{
	if (this != &o)
		*this = o;
}

RequestHttpBody& RequestHttpBody::operator=(const RequestHttpBody &o)
{
	this->content = o.content;
	this->len = o.len;
	return *this;
}

