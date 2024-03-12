#include "RequestHttp.h"

RequestHttp::~RequestHttp()
{
}

RequestHttp::RequestHttp(RequestHeader *head) :
		fdClient(-1)
{
	header = head;
}

void RequestHttp::dump()
{
	std::map<std::string, std::string>::iterator ite = kv.begin();
	while (ite != kv.end())
	{
		std::cout << ite->first << " -> " << ite->second << std::endl;
		ite++;
	}
}

const std::list<std::string> RequestHttp::getFields() const
{
	return header->getFields();
}

std::string RequestHttp::getUri()
{
	return header->getUri();
}
std::string RequestHttp::getMethod()
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
