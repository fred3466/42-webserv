#pragma once

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

#include "Harl.h"
#include "connector/Connector.h"
#include "connector/ConnectorFactory.h"

#define BUF_SIZE 1024

class HttpServer: public ConnectorListener {
private:
//	std::list<Connector> consListenersList;

	Connector *connector;

	Harl harl;
	std::map<int, int> _clients;

//	void _process_ready_for_read(int fwPort, int _soListen, netStruct NS);
//	void _listen(int _soListen, netStruct ns);
//	void _acceptIncomingCon(int new_sd, int &_soListen, struct pollfd fds[],
//			int &end_server, int &nfds);
//	bool _onDataReceiving(struct pollfd &curentPollFd, int &close_conn);
//
//	int _newClient(int _soListen, netStruct NS);
public:
	HttpServer();
	~HttpServer();

	void init(std::string ipStr, int port);
	void onIncomming(ConnectorEvent e);
	void onDataReceiving(ConnectorEvent e);

};

