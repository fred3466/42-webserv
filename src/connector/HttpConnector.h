#pragma once
#include <string>
#include <list>
#include "Connector.h"
#include "../Harl.h"

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

struct netStruct {

	int portServer;
	std::string ipServer;

};

class HttpConnector: public Connector {
private:
	Harl harl;
	netStruct ns;
	int _soListen;
//	std::list<ConnectorListener> connectorListenerList;
	ConnectorListener *connectorListener;
	std::list<int> _allSockets;

	HttpConnector(const HttpConnector &o);
	HttpConnector& operator=(const HttpConnector &o);

	void _acceptIncomingCon(int new_sd, int &_soListen, struct pollfd fds[],
			int &end_server, int &nfds);
	bool _onDataReceiving(struct pollfd &curentPollFd, int &close_conn);
	void _listen(int _soListen, netStruct ns);
public:
	HttpConnector();
	~HttpConnector();
	HttpConnector(std::string stIp, int port);
protected:
	void init(ConnectorListener &l);
	void doListen();

	void registerIt(ConnectorListener *l);
	void unregisterIt(ConnectorListener *l);
	void publishAccepting(ConnectorEvent e);
	void publishDataReceiving(ConnectorEvent e);

//	bool operator==(const HttpConnector &o);

};

