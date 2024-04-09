#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head, RequestBody *body) : fdClient(NULL), bConnectionKeepAlive(false)
{
	StringUtil su = StringUtil();
	header = head;
	this->body = body;

	std::string hostPort = getHeaderFieldValue("Host");
	std::vector<std::string> list = su.tokenize(hostPort, ':');
	host = su.getNthTokenIfExists(list, 0, "");
	port = su.intFromStr(su.getNthTokenIfExists(list, 1, "80"));

	if (head->getFields().empty())
	{
		throw "IllegalStateException : Request::getHeader().getFields() est vide !";
	}
	else
	{
		std::string connectionVal = header->getFieldValue("Connection");
		bConnectionKeepAlive = StringUtil().isStrictlyEqual(connectionVal, "keep-alive");
	}

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
//	return getHeaderFieldValue("Host");
	return host;
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

const Uri& RequestHttp::getUri() const
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

void RequestHttp::setBody(RequestBody *body)
{
	this->body = body;
}

RequestBody* RequestHttp::getBody()
{
	return body;
}

bool RequestHttp::isConnectionKeepAlive() throw (char*)
{
	return bConnectionKeepAlive;
}

int RequestHttp::getPort()
{
	return port;

}

std::string RequestHttp::getRemoteIpPort()
{

	return remoteIpPort;
}
