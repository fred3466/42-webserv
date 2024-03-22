#pragma once
#include <string>

#include "Connector.h"
#include "HttpConnector.h"
#include "../config/Config.h"

class ConnectorFactory
{
private:
	public:
	ConnectorFactory();
	~ConnectorFactory();
	Connector* build(std::string ip, int port, Config *c);
};
