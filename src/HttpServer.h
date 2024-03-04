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

#include "HttpServer.h"
#include "config/Config.h"
#include "config/ConfigFactory.h"
#include "connector/Connector.h"
#include "connector/ConnectorFactory.h"
#include "connector/TcpConnector.h"
#include "Harl.h"
#include "processor/Processor.h"
#include "processor/ProcessorFactory.h"
#include "request/Request.h"
#include "request/HttpRequest.h"
#include "request/RequestFactory.h"
#include "CGI/CGIHandler.h"

class HttpServer : public ConnectorListener
{
private:
	//	std::list<Connector> consListenersList;

	Connector *connector;
	Config *config;
	Harl harl;
	std::map<int, int> _clients;
	std::map<std::string, std::string> env;
	std::map<std::string, std::string> prepareCGIEnvironment(const HttpRequest &request);

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

	virtual void init(std::string ipStr, int port);
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