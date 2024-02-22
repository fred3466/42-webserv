#pragma once
#include <string>
#include "Connector.h"
#include "HttpConnector.h"

class ConnectorFactory
{
private:
//	ConnectorFactory(const ConnectorFactory &o);
//	ConnectorFactory& operator=(const ConnectorFactory &o);
public:
	ConnectorFactory();
	~ConnectorFactory();
	Connector* build(std::string ip, int port);

};

