#pragma once
#include <string>

#include "../API/Request.h"
#include "../API/Response.h"

class Connector;

class ConnectorEvent
{
private:
	int *fdClient;
	std::string eventName;
	char *byteBuffer;

public:
	ConnectorEvent(const ConnectorEvent &other);
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
//	ConnectorEvent(Request *req, Response *resp);
	ConnectorEvent(std::string eventName);
	~ConnectorEvent();
	void setByteBuffer(char *bytess);
	std::string getByteBuffer();
	void setFdClient(int *fd);
	int* getFdClient();
};

