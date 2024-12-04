#include "HttpConnector.h"

HttpConnector::HttpConnector() : harl(), ns(), _soListen(-1), connectorListenerList(), config(), _pollfd(NULL) {
}

HttpConnector::~HttpConnector() {
	harl.trace("HttpConnector::~HttpConnector : kill connectorListenerList : %i", connectorListenerList.size());
	if (_soListen != -1)
		close(_soListen);

}

HttpConnector::HttpConnector(std::string ipStr, int port, Config *c) : harl(), _soListen(-1), connectorListenerList(), _pollfd(NULL) {

	ns = netStruct();
	ns.ipServer = ipStr;
	ns.portServer = port;
	config = c;
}

void HttpConnector::registerIt(ResponseProducer *l) {
	if (l)
		connectorListenerList.push_back(l);
}
void HttpConnector::unregisterIt(ResponseProducer *l) {
	connectorListenerList.remove(l);
}
void HttpConnector::init(ResponseProducer &l) {
	registerIt(&l);
}

std::list<ResponseProducer*> HttpConnector::getResponseProducer() {
	return connectorListenerList;
}

int HttpConnector::start() {
	harl.trace2("---HttpConnector::start: socket");
	if ((_pollfd->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		harl.error("HttpConnector::start: Creating socket, bye !");
		return (0);
	}
	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	harl.trace2("---HttpConnector::start: setsockopt");

	int on = 1;
	int rc = setsockopt(_pollfd->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (rc < 0) {
		harl.error("HttpConnector::start : setsockopt() failed");
		close(_pollfd->fd);
		return (0);
	}

	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	harl.trace("---HttpConnector::start: ioctl");
	rc = ioctl(_pollfd->fd, FIONBIO, (char*) &on);
	if (rc < 0) {
		harl.error("HttpConnector::start : ioctl() failed");
		close(_pollfd->fd);
		return (0);
	}

	sockaddr_in sockaddrStruct;
	sockaddrStruct.sin_family = AF_INET;
	sockaddrStruct.sin_addr.s_addr = inet_addr(ns.ipServer.c_str());
	sockaddrStruct.sin_port = htons(ns.portServer);
	harl.trace("---HttpConnector::start: bind");
	if (bind(_pollfd->fd, reinterpret_cast<const sockaddr*>(&sockaddrStruct), sizeof(sockaddrStruct)) < 0)
			{
		harl.error("HttpConnector::start : bind error : [%s:%i] : [%s], bye !", ns.ipServer.c_str(), ns.portServer, strerror(errno));
		return (0);
	} else {
	}
	//	----------------------------------------------------------------------
	if (_pollfd->fd < 0) {
		harl.error("HttpConnector::start : bind error : [%s], bye !", strerror(errno));

		return (0);
	}

	harl.trace("---HttpConnector::start: listen");
	int backlog = 50;
	rc = listen(_pollfd->fd, backlog);
	if (rc < 0) {
		harl.error("HttpConnector::start : failed [%s]", strerror(errno));
		close(_pollfd->fd);
		return (0);
	}

	harl.info("!!!!!!! %i HttpConnector::start : Ecoute sur %s:%i", _pollfd->fd, ns.ipServer.c_str(), ns.portServer);

	return 1;
}

HttpConnector::HttpConnector(HttpConnector &bis) {
	if (this != &bis) {
		harl = bis.harl;
		ns = bis.ns;
		_soListen = bis._soListen;
		connectorListenerList = bis.connectorListenerList;
		config = bis.config;
		_pollfd = bis._pollfd;
	}

	*this = bis;
}
HttpConnector& HttpConnector::operator=(HttpConnector &bis) {
	if (this != &bis) {
		harl = bis.harl;
		ns = bis.ns;
		_soListen = bis._soListen;
		connectorListenerList = bis.connectorListenerList;
		config = bis.config;
		_pollfd = bis._pollfd;
	}
	return *this;
}

struct pollfd* HttpConnector::getListeningPollFd() {
	return _pollfd;
}

netStruct HttpConnector::getInternalNetStruct() {
	return ns;
}

void HttpConnector::setListeningPollFd(struct pollfd *pollfd)
		{
	_pollfd = pollfd;
}

int HttpConnector::getSoListen() const
{
	return _soListen;
}
