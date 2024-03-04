#pragma once
#include "Connector.h"

class TcpConnector : public Connector
{
private:
    int _listenFd; // Listening socket file descriptor
public:
    TcpConnector();
    virtual ~TcpConnector();

    virtual void doListen();
    int getListenFd() const;
};
