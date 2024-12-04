#pragma once
#include "ResponseProducer.h"

struct netStruct {

	int portServer;
	std::string ipServer;
};

class Connector /*: public ConnectorEventPublisher*/
{
private:

protected:
	public:
	Connector();
	virtual ~Connector();

	virtual int start()=0;
	virtual void registerIt(ResponseProducer *l)=0;
	virtual void unregisterIt(ResponseProducer *l)=0;
	virtual std::list<ResponseProducer*> getResponseProducer()=0;
	virtual pollfd* getListeningPollFd()=0;
	virtual void setListeningPollFd(struct pollfd *pollfd)=0;
	virtual netStruct getInternalNetStruct()=0;
	virtual int getSoListen() const =0;

};

