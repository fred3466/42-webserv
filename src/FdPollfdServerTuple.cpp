#include "FdPollfdServerTuple.h"

FdPollfdServerTuple::FdPollfdServerTuple()
: fd(-1), _pollfd(), httpServer(), bServerListening(false)
{
}

FdPollfdServerTuple::FdPollfdServerTuple(int fd, pollfd *_pollfd, HttpServer *httpServer, bool bServerListening)
: fd(fd), _pollfd(_pollfd), httpServer(httpServer), bServerListening(bServerListening)
{
}

pollfd* FdPollfdServerTuple::getPollfd() const
{
	return _pollfd;
}

void FdPollfdServerTuple::setPollfd(pollfd *pollfd)
{
	_pollfd = pollfd;
}

int FdPollfdServerTuple::getFd() const
{
	return fd;
}

void FdPollfdServerTuple::setFd(int fd)
{
	this->fd = fd;
}

HttpServer* FdPollfdServerTuple::getHttpServer() const
{
	return httpServer;
}

bool FdPollfdServerTuple::isBServerListening() const
{
	return bServerListening;
}

void FdPollfdServerTuple::setBServerListening(bool bServerListening)
{
	this->bServerListening = bServerListening;
}

void FdPollfdServerTuple::setHttpServer(HttpServer *httpServer)
{
	this->httpServer = httpServer;
}

FdPollfdServerTuple::~FdPollfdServerTuple()
{
	delete httpServer;
	delete _pollfd;
}

