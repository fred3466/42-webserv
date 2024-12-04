#pragma once
#include <string>

#include "../API/Connector.h"
#include "../config/Config.h"
#include "impl/HttpConnector.h"

class ConnectorFactory {
private:
public:
	ConnectorFactory();
	~ConnectorFactory();
	Connector* build(std::string ip, int port, Config *c);
};
