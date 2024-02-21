/*
 * HttpServer.cpp
 *
 *  Created on: 5 févr. 2024
 *      Author: fbourgue
 */

#include "HttpServer.h"

HttpServer::HttpServer() :
		harl() {

}

HttpServer::~HttpServer() {
}

//bool HttpServer::operator==(const ConnectorListener &o) {
////	if (o)
//	return this->_soListen ==  o._soListen;
////	return 0;
//}
void shutFd(int fd) {
	if (fd >= 0) {
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}
void HttpServer::init(std::string ipStr, int port) {
	netStruct ns;
	connector = ConnectorFactory().build(ipStr, port);
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::onIncomming(ConnectorEvent e) {

}
void HttpServer::onDataReceiving(ConnectorEvent e) {
//	int len = rc;
//	harl.debug("  %d bytes received", len);
	std::cout << e.getTemp();

//	rc = send(curentPollFd.fd, buffer, len, 0);
//	if (rc < 0) {
//		harl.error("  send() failed");
//		close_conn = 1;
//		break;
//	}
}
