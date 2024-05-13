#pragma once
#include "../../API/Connector.h"

class TcpConnector: public Connector
{
private:
	int _listenFd; // Listening socket file descriptor
public:
	TcpConnector();
	virtual ~TcpConnector();

	virtual void doPoll();
	int getListenFd() const;
};
