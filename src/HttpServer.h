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
#include "processor/Processor.h"
#include "processor/ProcessorFactory.h"
#include "request/API/Request.h"
#include "request/factory/RequestFactory.h"
#include "request/API/RequestHeader.h"
#include "request/factory/RequestHeaderFactory.h"
#include "location/ProcessorLocator.h"

class HttpServer: public ConnectorListener
{
private:
	//	std::list<Connector> consListenersList;

	Connector *connector;
	Config *config;
	Harl harl;
	ProcessorFactory processorFactory;
	//	ProcessorLocator processorLocator;

public:
	HttpServer();
	~HttpServer();

	virtual void init(Config *conf);
	virtual void onIncomming(ConnectorEvent e);
	virtual void onDataReceiving(ConnectorEvent e);

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

