#include "ConnectorEvent.h"

ConnectorEvent::~ConnectorEvent()
{
	delete[] byteBuffer;
}

ConnectorEvent::ConnectorEvent(const ConnectorEvent &o) : fdClient(o.fdClient), byteBuffer(o.byteBuffer), fdPollfdServerTuple(o.fdPollfdServerTuple)
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

ConnectorEvent::ConnectorEvent(std::string eventName) : fdClient(NULL), byteBuffer(NULL), len(0), fdPollfdServerTuple()
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

std::string const& ConnectorEvent::getEventName() const
{
	return eventName;
}

FdPollfdServerTuple* ConnectorEvent::getFdPollfdServerTuple()
{
	return fdPollfdServerTuple;
}

void ConnectorEvent::setFdPollfdServerTuple(FdPollfdServerTuple *fdPollfdServerTuple)
{
	this->fdPollfdServerTuple = fdPollfdServerTuple;
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
