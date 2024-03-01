#include "ConnectorFactory.h"

ConnectorFactory::ConnectorFactory()
{

}

ConnectorFactory::~ConnectorFactory()
{
}

Connector* ConnectorFactory::build(std::string ip, int port)
{
	return new HttpConnector(ip, port);
}

