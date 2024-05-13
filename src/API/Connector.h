#pragma once
#include <sys/poll.h>
#include "ConnectorEventPublisher.h"
#include "ConnectorListener.h"

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
	//	virtual void doPoll()=0;

	virtual void registerIt(ConnectorListener *l)=0;
	virtual void unregisterIt(ConnectorListener *l)=0;
	virtual void publishAccepting(ConnectorEvent *e)=0;
	virtual void publishDataReceiving(ConnectorEvent *e)=0;
	//	virtual void closeConnection(int *fd)=0;
	virtual pollfd* getPollFd()=0;
//	virtual int getFdTabSize()=0;

};

