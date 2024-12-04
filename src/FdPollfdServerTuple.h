#pragma once

#include <sys/poll.h>

class Server;

enum FdPollfdServerTupleTypeEnum {
	LISTENING_SOCKET_SERVER_TYPE, COMMON_SOCKET_SERVER_TYPE, PIPE_TYPE, NOT_SET_TYPE
};

class Request;
class Response;

class FdPollfdServerTuple {
private:
	pollfd *_pollfd;
	Server *httpServer;
	FdPollfdServerTupleTypeEnum typeEnum;
	bool bSocketConnectionCloseRequired;
	bool bSocketKeepAlive;
	int nbWritten;
	Response *response;
	Request *request;
	bool bTooBigError;

	FdPollfdServerTuple();

public:
	FdPollfdServerTuple(pollfd *_pollfd, Server *httpServer, FdPollfdServerTupleTypeEnum typeEnum);
	~FdPollfdServerTuple();
	FdPollfdServerTuple(FdPollfdServerTuple &bis);
	FdPollfdServerTuple& operator=(FdPollfdServerTuple &bis);
	pollfd* getPollfd();
	void setPollfd(pollfd *pollfd);
	int getFd();
	void setFd(int fd);
	Server* getHttpServer();
	void setHttpServer(Server *httpServer);
	bool isBConnectionCloseRequired();
	void setBConnectionCloseRequired(bool bConnectionCloseRequired);
	bool isBKeepAlive() const;
	void setBKeepAlive(bool bKeepAlive);
	int getNbWritten() const;
	void setNbWritten(int nbWritten);
	FdPollfdServerTupleTypeEnum getTypeEnum() const;
	void setTypeEnum(FdPollfdServerTupleTypeEnum typeEnum);
	Response* getResponse();
	void setResponse(Response *response);
	Request* getRequest();
	void setRequest(Request *request);
	bool isBTooBigError() const;
	void setBTooBigError(bool bTooBigError);
};

