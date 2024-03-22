#include "ConnectorEvent.h"

ConnectorEvent::~ConnectorEvent()
{
}

ConnectorEvent::ConnectorEvent(const ConnectorEvent &o) :
		req(o.req), resp(o.resp), byteBuffer(o.byteBuffer)
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

ConnectorEvent::ConnectorEvent(Request *req, Response *resp) :
		fdClient(NULL), resp(NULL)
{
	this->req = req;
	this->resp = resp;
}

ConnectorEvent::ConnectorEvent(std::string temp) :
		fdClient(NULL), resp(NULL), req(NULL)
{
	this->byteBuffer = temp;
}

std::string ConnectorEvent::getByteBuffer()
{
	return byteBuffer;
}

int* ConnectorEvent::getFdClient()
{
	return fdClient;
}

void ConnectorEvent::setFdClient(int *fd)
{
	fdClient = fd;
}
