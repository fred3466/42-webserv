#pragma once
#include <string>
#include <list>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <map>
#include <list>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "../../API/ResponseProducer.h"
#include "../../API/Connector.h"
#include "../../Harl.h"
#include "../../config/Config.h"

class HttpConnector: public Connector {
private:
	Harl harl;
	netStruct ns;
	int _soListen;
	std::list<ResponseProducer*> connectorListenerList;
	Config *config;
	struct pollfd *_pollfd;

protected:
	void init(ResponseProducer &l);

public:
	HttpConnector();
	~HttpConnector();
	HttpConnector(std::string stIp, int port, Config *c);
	HttpConnector(HttpConnector &bis);
	HttpConnector& operator=(HttpConnector &bis);

	virtual int start();
	virtual void registerIt(ResponseProducer *l);
	virtual void unregisterIt(ResponseProducer *l);
	virtual std::list<ResponseProducer*> getResponseProducer();
	virtual pollfd* getListeningPollFd();
	virtual void setListeningPollFd(struct pollfd *pollfd);
	virtual netStruct getInternalNetStruct();
	virtual int getSoListen() const;
};
