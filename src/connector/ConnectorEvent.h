#pragma once
#include <string>

#include "../request/API/Request.h"
#include "../response/API/Response.h"

class ConnectorEvent
{
private:
	Request *req;
	Response *resp;
	std::string temp;
	int fdClient;

public:
	ConnectorEvent(const ConnectorEvent &other);
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
	ConnectorEvent(Request *req, Response *resp);
	ConnectorEvent(std::string temp);
	~ConnectorEvent();
	std::string getTemp();
	void setFdClient(int fd);
	int getFdClient();
};

