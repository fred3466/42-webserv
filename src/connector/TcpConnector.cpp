#include "TcpConnector.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// The constructor initializes _listenFd to an invalid value (-1).
// The destructor closes _listenFd if it's a valid socket descriptor.
// This ensures that the socket is properly closed when the TcpConnector instance is destroyed.

TcpConnector::TcpConnector() : _listenFd(-1)
{
    //  initialize the listening socket
    // or leave it to the doListen() method
}

TcpConnector::~TcpConnector()
{
    if (_listenFd >= 0)
    {
        close(_listenFd);
    }
}

void TcpConnector::doListen()
{
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd < 0)
    {
        std::cerr << "Error opening socket." << std::endl;
        return;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any IP address
    serv_addr.sin_port = htons(8080);       // Specify port

    if (bind(_listenFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Error on binding." << std::endl;
        return;
    }

    listen(_listenFd, 5); // Start listening on the socket
    std::cout << "Listening on port 8080" << std::endl;
}

int TcpConnector::getListenFd() const
{
    return _listenFd;
}
