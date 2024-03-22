#include "ConnectorFactory.h"

ConnectorFactory::ConnectorFactory()
{
}

ConnectorFactory::~ConnectorFactory()
{
}

Connector* ConnectorFactory::build(std::string ip, int port, Config *config)
{
	return new HttpConnector(ip, port, config);
}
