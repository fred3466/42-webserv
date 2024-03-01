#pragma once
#include "ConnectorEventPublisher.h"

class Connector /*: public ConnectorEventPublisher*/
{
private:
	protected:
	//	Connector(const Connector &other);
//	Connector& operator=(const Connector &other);
public:
	Connector();
	//	Connector(std::string stIp, int port);
	virtual ~Connector();
	virtual void doListen()=0;

	virtual void registerIt(ConnectorListener *l)=0;
	virtual void unregisterIt(ConnectorListener *l)=0;
	virtual void publishAccepting(ConnectorEvent e)=0;
	virtual void publishDataReceiving(ConnectorEvent e)=0;

};

