#pragma once
#include <string>

#include "../API/Request.h"
#include "../API/Response.h"

class Connector;

class ConnectorEvent
{
private:
	Request *req;
	Response *resp;
	int *fdClient;
	std::string byteBuffer;

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

