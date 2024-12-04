#pragma once

#include "../config/Config.h"
#include "Connector.h"

class Server: public ResponseProducer {
public:
	Server();
	virtual ~Server();
	virtual void init(Config *conf)=0;
	virtual Connector* getConnector()=0;
	virtual Config*& getConfig()=0;
	virtual void setConnector(Connector *connector)=0;
};

