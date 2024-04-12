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

#include "config/Config.h"
#include "config/ConfigFactory.h"
#include "connector/Connector.h"
#include "connector/ConnectorFactory.h"
#include "Harl.h"
#include "processor/ProcessorFactory.h"
#include "request/API/Request.h"
#include "request/factory/RequestFactory.h"
#include "request/factory/RequestBodyFactory.h"
#include "request/API/RequestHeader.h"
#include "request/factory/RequestHeaderFactory.h"
#include "location/ProcessorLocator.h"
#include "parser/MultipartParser.h"
#include "cookie/Cookie.h"
#include "cookie/factory/CookieFactory.h"
#include "error/HttpErrorFactory.h"
#include "processor/API/Processor.h"
// #include "response/API/ResponseHeader.h"

// class ResponseHeader;
class HttpServer : public ConnectorListener
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

public:
	HttpServer();
	~HttpServer();

	virtual void init(Config *conf);
	virtual void onIncomming(ConnectorEvent e);
	virtual void onDataReceiving(ConnectorEvent e);

	bool checkRequestBodySize(Request *request, Response *&response);

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

// yes "This is a test. " | head -c 5000 | curl -X POST -H "Content-Type: plain/text" --data-binary @- http://s1.org:8081/cgi-bin_fred/anastasia.php

// curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://s1.org:8081/cgi-bin_fred/anastasia.php