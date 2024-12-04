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
#include <time.h>
#include <ostream>

#include "API/Connector.h"
#include "API/Processor.h"
#include "API/Request.h"
#include "API/RequestHeader.h"
#include "API/Server.h"
#include "config/Config.h"
#include "config/ConfigFactory.h"
#include "connector/ConnectorFactory.h"
//#include "connector/RequestResponseParam.h"
#include "Harl.h"
#include "processor/ProcessorFactory.h"
#include "request/RequestHelper.h"
#include "request/factory/RequestFactory.h"
#include "request/factory/RequestBodyFactory.h"
#include "request/factory/RequestHeaderFactory.h"
#include "location/ProcessorLocator.h"
#include "parser/MultipartParser.h"
#include "cookie/Cookie.h"
#include "cookie/CookieHelper.h"
#include "cookie/factory/CookieFactory.h"
#include "error/HttpErrorFactory.h"
#include "error/HttpReturnCodeHelper.h"
#include "API/ResponseProducer.h"

class FdPollfdServerTuple;
class HttpServer: public Server {
private:
	std::string name;
	static Harl harl;
	Connector *connector;
	Config *config;
	static ProcessorLocator *processorLocator;
	static StringUtil su;
	HttpErrorFactory errorFactory;
	RequestHelper *requestHelper;
	bool bShareConnector;
	static std::vector<ProcessorAndLocationToProcessor*> *processorList;

	void instantiateProcessLocator();
	bool _checkAccess(Request *request, Processor *processor);
	bool checkRequestBodySize(Request *request, Response *&response);

public:
	HttpServer();
	~HttpServer();

	virtual void init(Config *conf);
	virtual Response* createResponse(RequestResponseParam *requestResponseParam);
	virtual void setConnector(Connector *connector);

	static void freeProcessorList();
	Connector* getConnector();
	Config*& getConfig();
	std::string getName();
	bool isBShareConnector() const;
	static Response* runProcessorChain(Request *request, Response *resp,
			ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor);
	static Response* createEmptyResponse();
};
