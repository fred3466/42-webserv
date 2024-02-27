/*
 * HttpServer.cpp
 *
 *  Created on: 5 févr. 2024
 *      Author: fbourgue
 */

#include "HttpServer.h"

HttpServer::HttpServer() :
		harl(), connector()
{

}

HttpServer::~HttpServer()
{
}

//bool HttpServer::operator==(const ConnectorListener &o) {
////	if (o)
//	return this->_soListen ==  o._soListen;
////	return 0;
//}
void shutFd(int fd)
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}
void HttpServer::init(std::string ipStr, int port)
{
	netStruct ns;

	config = ConfigFactory().build();

//	config.read("config.properties");

	connector = ConnectorFactory().build(ipStr, port);
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::onIncomming(ConnectorEvent e)
{

}
void HttpServer::onDataReceiving(ConnectorEvent e)
{
//	std::cout << e.getTemp();
	std::string rawRequest = e.getTemp();
	Request *request = RequestFactory().build(&rawRequest);
	request->setFdClient(e.getFdClient());
//	req->dump();

//	Validator *validator = ValidatorFactory().build(req);
//	validator->validate(req);

	Processor *processor = ProcessorFactory().build(request);
	processor->setConfig(config);
	Response *resp = processor->process(request);

//	int length = resp->getStatusLine().length() + 1;
//	char *cstr = new char[length];
//	std::strcpy(cstr, resp->getStatusLine().c_str());
//	send(request->getFdClient(), cstr, length - 1, 0);
//
//	length = resp->getHeader().length() + 1;
//	cstr = new char[length];
//	std::strcpy(cstr, resp->getHeader().c_str());
//	send(request->getFdClient(), cstr, length - 1, 0);
//
//	length = resp->getBody().length() + 1;
//	cstr = new char[length];
//	std::strcpy(cstr, resp->getBody().c_str());
//	send(request->getFdClient(), cstr, length - 1, 0);

//Send Response
	std::string statusHeaderBody = resp->getStatusLine() + resp->getHeader()
			+ resp->getBody();
	int length = resp->getStatusLine().length() + resp->getHeader().length()
			+ resp->getBodyLength() + 1;
	char *cstr = new char[length];
	std::strcpy(cstr, statusHeaderBody.c_str());
	send(request->getFdClient(), cstr, length, 0);
//	write(request->getFdClient(), cstr, length);
//	rc = send(curentPollFd.fd, buffer, len, 0);
//	if (rc < 0) {
//		harl.error("  send() failed");
//		close_conn = 1;
//		break;
//	}
}
