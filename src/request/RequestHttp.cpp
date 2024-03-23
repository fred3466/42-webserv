#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head) : fdClient(NULL)
{
	header = head;
}

RequestHeader* RequestHttp::getHeader() const
{
	return header;
}

const std::string RequestHttp::getFileExtension() const
{
	return header->getFileExtension();
}

const std::string RequestHttp::getFileName() const
{
	return header->getFileName();
}

const std::string RequestHttp::getHost()
{
	return getHeaderFieldValue("Host");
}

const std::string& RequestHttp::getPath()
{
	return header->getPath();
}

const std::string& RequestHttp::getQueryString() const
{
	return header->getQueryString();
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

const std::string& RequestHttp::getUri() const
{
	return header->getUri();
}

const std::string& RequestHttp::getMethod() const
{
	return header->getMethod();
}

int* RequestHttp::getFdClient() const
{
	return fdClient;
}
void RequestHttp::setFdClient(int *fd)
{
	fdClient = fd;
}
bool RequestHttp::isConnectionKeepAlive() throw (char*)
{
	bool ret = false;
	StringUtil su = StringUtil();
	RequestHeader *header = getHeader();
	if (header->getFields().empty())
	{
		throw "IllegalStateException : Request::getHeader().getFields() est vide !";
	} else
	{
		std::string connectionVal = header->getFieldValue("Connection");
		ret = su.isStrictlyEqual(connectionVal, "keep-alive");
	}
	return ret;
}
