#include "ConnectorFactory.h"

ConnectorFactory::ConnectorFactory() {

}

ConnectorFactory::~ConnectorFactory() {
}

ConnectorFactory::ConnectorFactory(const ConnectorFactory &o) {
	*this = o;
}

ConnectorFactory& ConnectorFactory::operator=(const ConnectorFactory &o) {
	if (this != &o)
		*this = o;
	return *this;

}

Connector* ConnectorFactory::build(std::string ip, int port) {
	return new HttpConnector(ip, port);
}

