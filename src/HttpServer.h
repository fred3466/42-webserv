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

#include "API/Connector.h"
#include "API/Processor.h"
#include "API/Request.h"
#include "API/RequestHeader.h"
#include "config/Config.h"
#include "config/ConfigFactory.h"
#include "connector/ConnectorFactory.h"
#include "Harl.h"
#include "processor/ProcessorFactory.h"
#include "request/factory/RequestFactory.h"
#include "request/factory/RequestBodyFactory.h"
#include "request/factory/RequestHeaderFactory.h"
#include "location/ProcessorLocator.h"
#include "parser/MultipartParser.h"
#include "cookie/Cookie.h"
#include "cookie/factory/CookieFactory.h"
#include "error/HttpErrorFactory.h"
#include "error/HttpReturnCodeHelper.h"
#include "Server.h"

// class ResponseHeader;
class HttpServer : public ConnectorListener, public Server
{
private:
	//	std::list<Connector> consListenersList;

	Harl harl;
	Connector *connector;
	Config *config;
	ProcessorLocator *processorLocator;
	StringUtil su;
	Response *runProcessorChain(std::vector<ProcessorAndLocationToProcessor *> *processorList, Request *request,
								Response *resp);
	char *packageResponseAndGiveMeSomeBytes(Request *request, Response *resp);
	int pushItIntoTheWire(int *fdSocket, Request *request, Response *resp);
	void cleanUp(Request *request, Response *resp);
	void instantiateProcessLocator();
	void addUltimateHeaders(Response *resp);
	bool _checkAccess(Request *request);
	bool checkRequestBodySize(Request *request, Response *&response);

public:
	HttpServer();
	~HttpServer();

	virtual void init(Config *conf);
	virtual void onIncomming(ConnectorEvent e);
	virtual void onDataReceiving(ConnectorEvent e);

	Response *createErrorResponse(int errorCode);
	Response *handleHttpError(int errorCode);

	//	ProcessorLocator getProcessorLocator();
	//	void addLocationToProcessor(std::string ext, Processor *processor);

	//	std::string readRequest(int clientFd);
	//	void sendResponse(int clientFd, const std::string &response);
	//	void closeClient(int clientFd);
	//	int getListenFd();
	//	bool isCGIRequest(const std::string &uri);
	//	std::string getScriptPath(const std::string &uri);
	//	std::string generateHttpResponse(const std::string &cgiOutput);
	//	int getClientFd(int clientId);
};

// yes "This is a test. " | head -c 5000 | curl -X POST -H "Content-Type: plain/text" --data-binary @- http://s2.org:8082/cgi-bin_fred/anastasia.php

// curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://s2.org:8082/cgi-bin_fred/anastasia.php

// - Create a string with exactly 100 bytes
// data=$(printf '%*s' 100 | tr ' ' 'x')

// - Send the request
// curl -X POST -d "$data" http://127.0.0.2:8082/post_body

// - Create a string with 101 bytes
// data=$(printf '%*s' 101 | tr ' ' 'x')

// - Send the request
// curl -X POST -d "$data" http://127.0.0.2:8082/post_body
