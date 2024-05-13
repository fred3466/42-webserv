#pragma once

#include <sys/poll.h>
#include "HttpServer.h"

//class HttpServer;

class FdPollfdServerTuple
{
private:
	int fd;
	pollfd *_pollfd;
	HttpServer *httpServer;
	bool bServerListening;

public:
	FdPollfdServerTuple();
	FdPollfdServerTuple(int fd, pollfd *_pollfd, HttpServer *httpServer, bool bServerListening);
	~FdPollfdServerTuple();
	pollfd* getPollfd() const;
	void setPollfd(pollfd *pollfd);
	int getFd() const;
	void setFd(int fd);
	HttpServer* getHttpServer() const;
	void setHttpServer(HttpServer *httpServer);
	bool isBServerListening() const;
	void setBServerListening(bool bServerListening);
};

