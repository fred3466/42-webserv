#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head) :
		fdClient(-1)
{
	header = head;
}

void RequestHttp::dump() const
{
	std::map<std::string, std::string>::const_iterator ite = kv.begin();
	while (ite != kv.end())
	{
		std::cout << ite->first << " -> " << ite->second << std::endl;
		ite++;
	}
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
