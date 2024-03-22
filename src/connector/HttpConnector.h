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

#include "ConnectorListener.h"
#include "Connector.h"
#include "../Harl.h"
#include "../config/Config.h"

struct netStruct
{

	int portServer;
	std::string ipServer;
};

class HttpConnector: public Connector
{
private:
	Harl harl;
	netStruct ns;
	int _soListen;
	//	std::list<ConnectorListener> connectorListenerList;
	ConnectorListener *connectorListener;
	std::list<int> _allSockets;
	Config *config;

	//	HttpConnector(const HttpConnector &o);
	//	HttpConnector& operator=(const HttpConnector &o);

	void _acceptIncomingCon(int new_sd, int &_soListen, struct pollfd fds[],
			int &end_server, int &nfds);
	bool _onDataReceiving(struct pollfd *curentPollFd, int &close_conn);
	void _listen(int _soListen, netStruct ns);
	//	void reorganiseFdTab(struct pollfd *fdTab[], int *fdTabSize);
	void reorganiseFdTab(pollfd **fdTab, int *fdTabSize);

public:
	HttpConnector();
	~HttpConnector();
	HttpConnector(std::string stIp, int port, Config *c);

protected:
	void init(ConnectorListener &l);
	void doListen();

	virtual void registerIt(ConnectorListener *l);
	virtual void unregisterIt(ConnectorListener *l);
	virtual void publishAccepting(ConnectorEvent e);
	virtual void publishDataReceiving(ConnectorEvent e);
	virtual void closeConnection(int *fd);
};
