#pragma once
#include <string>

#include "Connector.h"
#include "HttpConnector.h"

class ConnectorFactory
{
private:
public:
	ConnectorFactory();
	~ConnectorFactory();
	Connector *build(std::string ip);
	Connector *build(std::string ip, int port);
};
