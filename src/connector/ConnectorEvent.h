#pragma once
#include <string>

#include "../request/API/Request.h"
#include "../response/API/Response.h"

class Connector;

class ConnectorEvent
{
private:
	Request *req;
	Response *resp;
	std::string byteBuffer;
	int *fdClient;

public:
	ConnectorEvent(const ConnectorEvent &other);
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
	ConnectorEvent(Request *req, Response *resp);
	ConnectorEvent(std::string temp);
	~ConnectorEvent();
	std::string getByteBuffer();
	void setFdClient(int *fd);
	int* getFdClient();
};

