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

#include "../../API/ConnectorListener.h"
#include "../../API/Connector.h"
#include "../../Harl.h"
#include "../../config/Config.h"

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
	ConnectorListener *connectorListener;
	Config *config;
	struct pollfd *_pollfd;

//	void _acceptIncomingCon(int new_sd, int &_soListen, struct pollfd fds[],
//			int &end_server, int &nfds);
//	bool _onDataReceiving(struct pollfd *curentPollFd, int &close_conn);
//	void _listen(int _soListen, netStruct ns);
//	void reorganiseFdTab(pollfd **fdTab, int *fdTabSize);
//	void dumpFdTab(pollfd **fdTab, int fdTabSize);
//	void signalHandler(int signum);

public:
	HttpConnector();
	~HttpConnector();
	HttpConnector(std::string stIp, int port, Config *c);

	//	virtual void closeConnection(int *fd);

protected:
	void init(ConnectorListener &l);
	//	void doPoll();

	virtual void registerIt(ConnectorListener *l);
	virtual void unregisterIt(ConnectorListener *l);
	virtual void publishAccepting(ConnectorEvent *e);
	virtual void publishDataReceiving(ConnectorEvent *e);
	virtual pollfd* getPollFd();
//	virtual int getFdTabSize();
};
