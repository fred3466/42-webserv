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

#include "HttpServer.h"
#include "config/Config.h"
#include "config/ConfigFactory.h"
#include "connector/Connector.h"
#include "connector/ConnectorFactory.h"
#include "connector/TcpConnector.h"
#include "Harl.h"
#include "processor/Processor.h"
#include "processor/ProcessorFactory.h"
#include "request/API/Request.h"
#include "request/factory/RequestFactory.h"
#include "request/API/RequestHeader.h"
#include "request/factory/RequestHeaderFactory.h"

class HttpServer: public ConnectorListener
{
private:
	//	std::list<Connector> consListenersList;

	Connector *connector;
	Config *config;
	Harl harl;
	public:
	HttpServer();
	~HttpServer();

	virtual void init(Config conf);
	virtual void onIncomming(ConnectorEvent e);
	virtual void onDataReceiving(ConnectorEvent e);
	//	virtual bool operator==(const ConnectorListener &other);

	std::string readRequest(int clientFd);
	void sendResponse(int clientFd, const std::string &response);
	void closeClient(int clientFd);
	int getListenFd();
	bool isCGIRequest(const std::string &uri);
	std::string getScriptPath(const std::string &uri);
	std::string generateHttpResponse(const std::string &cgiOutput);
	int getClientFd(int clientId);
};
