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
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <csignal>

#include "config.h"
#include "API/Server.h"
#include "HttpServer.h"
#include "FdPollfdServerTuple.h"
#include "util/FileUtil.h"
#include "Uri/Uri.h"
#include "connector/ConnectorBuffer.h"
#include "connector/impl/HttpConnector.h"
#include "ServerRunner.h"

class PollManager {
private:
	static bool _SIGINT;

	static Harl harl;
	static struct pollfd _pollFds[POLLFD_LIMIT];
	static int nbPollFds;
	static std::list<FdPollfdServerTuple*> fdPollfdServerTuple_List;

	static void removeTupleForFd(int fd);
	static void reorganisePollFds();
	static FdPollfdServerTuple* acceptIncomingCon(FdPollfdServerTuple *fdPollfdServerTuple);
	static void clearPollFds(pollfd *pfd);
	static int findFreePollFd();
	static void pollFdsReset(const int &index);
	static bool _readAvailable(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd);
	static bool _writeAvailable(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd);

public:
	static std::list<Server*> httpServerList;
	//	static int signalPipe;
	PollManager();
	~PollManager();

	static void dumpPollFds(std::string intro);
	static FdPollfdServerTuple* selectTupleForFd(int fd);
	static void closeAndReleasePollFd(pollfd *_pollfd);
	static void pollFdsInit();
	static int run(std::vector<Config*> configVector);
	static void doPoll(bool bOnlyPipes);
	static std::list<FdPollfdServerTuple*>* getFdPollfdServerTupleList();
	static void cleanup();
	static FdPollfdServerTuple* registerIncomingSocket(int new_fd, Server *server, bool bServerListening, bool bWriteModeToo);
	static FdPollfdServerTuple* registerPipe(int fdPipe, bool bIsInputTypeg);
	static FdPollfdServerTuple* setPollFdWritable(int fd);
};

