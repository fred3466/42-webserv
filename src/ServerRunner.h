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
#include "PollManager.h"
#include "processor/CGI/CGIHelper.h"

class ServerRunner {
private:
	static Harl harl;
	static std::vector<Config*> configVector;
	static std::list<FdPollfdServerTuple*> fdPollfdServerTuple_List;

	static void signal_handler(int signal);
	static RequestResponseParam createRequestInEvent(RequestResponseParam e);
	static int prepareToPushItIntoTheWire(int fdSocket, Request *request, Response *resp);
	static char* packageResponseAndGiveMeSomeBytes(Request *request, Response *resp);
	static void addUltimateHeaders(Response *resp);
	static Response* createErrorResponse(int errorCode/*, Request *request*/);
	static Request* buildCompleteResponse(ConnectorBuffer *connectorBuffer_READ, FdPollfdServerTuple *fdPollfdServerTuple);

public:
	static int nbResp;
	static bool bAlreadyDelResped;
	static std::list<Server*> httpServerList;
	static bool _SIGINT;

	ServerRunner();
	~ServerRunner();
	static Connector* isExistingServerConnector(Connector *con);
	static void _onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple);
	static bool _onWritingEnabled(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd);
	static void sendResponse(FdPollfdServerTuple *fdPollfdServerTuple, Request *request, Response *resp);
	static int run(std::vector<Config*> configVector);
	static void cleanup();
	static void delRequest(Request *request);
	static void delResp(Response *resp);
	static void cleanUpRequestResponse(Request *request, Response *resp);
	static std::list<FdPollfdServerTuple*>* getFdPollfdServerTupleList();
	static void sendErrorBodyTooBig(FdPollfdServerTuple *fdPollfdServerTuple);

};

