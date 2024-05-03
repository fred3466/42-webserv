#include "ConnectorEvent.h"

ConnectorEvent::~ConnectorEvent()
{
}

ConnectorEvent::ConnectorEvent(const ConnectorEvent &o) : fdClient(o.fdClient), byteBuffer(o.byteBuffer)
{
	*this = o;
}

//ConnectorEvent& ConnectorEvent::operator=(const ConnectorEvent &o) {
//	if (this != &o) {
//		req = o.req;
//		resp = o.resp;
//		temp = o.temp;
//		*this = o;
//	}
//	return *this;
//}

ConnectorEvent::ConnectorEvent(std::string eventName) : fdClient(NULL), byteBuffer(NULL)
{
	this->eventName = eventName;
}

char* ConnectorEvent::getByteBuffer()
{
	return byteBuffer;
}

void ConnectorEvent::setByteBuffer(char *bytess)
{
	byteBuffer = bytess;
}

int ConnectorEvent::getLen()
{
	return len;
}

void ConnectorEvent::setLen(int len)
{
	this->len = len;
}

int* ConnectorEvent::getFdClient()
{
	return fdClient;
}

void ConnectorEvent::setFdClient(int *fd)
{
	fdClient = fd;
}
