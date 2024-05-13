#pragma once

#include "config.h"
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
#include <sys/poll.h>
#include <sys/socket.h>

#include "HttpServer.h"
#include "FdPollfdServerTuple.h"
#include "util/FileUtil.h"
#include "Uri/Uri.h"

class ServerRunner
{
private:
	//	pollfd			_pollFds[POLLFD_LIMIT];
	struct pollfd *_pollFds[POLLFD_LIMIT];
	int nbPollFds;
	std::vector<Config*> configVector;
	std::list<HttpServer*> httpServerList;
	std::list<FdPollfdServerTuple*> fdPollfdServerTuple_List;

	void _acceptIncomingCon(FdPollfdServerTuple *fdPollfdServerTuple);
	bool _onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple, int &close_conn);
	void closeConnection(FdPollfdServerTuple *fdPollfdServerTuple);
	void removeTupleForFd(int fd);
	void reorganiseFdTab();

public:
	ServerRunner();
	~ServerRunner();

	void run(std::vector<Config*> configVector);
	void doPoll();

};

