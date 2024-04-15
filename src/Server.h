#pragma once

#include "config/Config.h"
#include "connector/ConnectorEvent.h"

class Server
{
public:
	Server();
	virtual ~Server();
	virtual void init(Config *conf)=0;
	virtual void onIncomming(ConnectorEvent e)=0;
	virtual void onDataReceiving(ConnectorEvent e)=0;

};

