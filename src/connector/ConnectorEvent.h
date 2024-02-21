#pragma once
#include <string>
#include "../request/Request.h"
#include "../response/Response.h"

class ConnectorEvent {
private:
	Request req;
	Response resp;
	std::string temp;

public:
	ConnectorEvent(const ConnectorEvent &other);
//	ConnectorEvent& operator=(const ConnectorEvent &other);
	ConnectorEvent(Request req, Response resp);
	ConnectorEvent(std::string temp);
	~ConnectorEvent();
	std::string getTemp();
};

