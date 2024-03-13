#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head) :
		fdClient(-1)
{
	header = head;
}

//void RequestHttp::dump() const
//{
//	std::map<std::string, std::string>::const_iterator ite = kv.begin();
//	while (ite != kv.end())
//	{
//		std::cout << ite->first << " -> " << ite->second << std::endl;
//		ite++;
//	}
//}

RequestHeader* RequestHttp::getHeader() const
{
	return header;
}

std::string RequestHttp::getFileExtension() const
{
	StringUtil su = StringUtil();
	std::string uri = getUri();
	std::string fileExt = uri.substr(uri.rfind(".", std::string::npos));
	if (!fileExt.empty())
	{
		return su.strUpper(fileExt);
	}
	return "";
}
std::string RequestHttp::getQueryString() const
{
	std::string queryString = ""; // Initialize queryString to an empty string
	std::string uri = getUri();
	size_t queryPos = uri.find('?');
	if (queryPos != std::string::npos)
	{
		queryString = uri.substr(queryPos + 1);
	}
	return queryString;
}

std::string RequestHttp::getHeaderFieldValue(std::string fieldName) const
{
	std::string fieldValue = header->getFieldValue(fieldName);
	return fieldValue;
}

void RequestHttp::addField(std::string rawField) const
{
	getHeader()->addField(rawField);
}

const std::list<std::string>& RequestHttp::getFields() const
{
	return header->getFields();
}

std::string RequestHttp::getUri() const
{
	return header->getUri();
}
std::string RequestHttp::getMethod() const
{
	return header->getMethod();
}

int RequestHttp::getFdClient() const
{
	return fdClient;
}
void RequestHttp::setFdClient(int fd)
{
	fdClient = fd;
}
