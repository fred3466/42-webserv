#include "HttpConnector.h"

HttpConnector::HttpConnector() : harl(), ns(), _soListen(-1), connectorListener(NULL), config(), fdTab(NULL), fdTabSize(0)
{
}

HttpConnector::~HttpConnector()
{
}

// HttpConnector::HttpConnector(const HttpConnector &o)
//{
//	*this = o;
// }

// bool HttpConnector::operator==(const HttpConnector &o)
//{
////	if (o)
//	return this->_soListen == o._soListen;
////	return 0;
//}

void HttpConnector::registerIt(ConnectorListener *l)
{
	//	connectorListenerList.push_back(*l);
	connectorListener = l;
}
void HttpConnector::unregisterIt(ConnectorListener *l)
{
	//	connectorListenerList.remove(*l);
	(void) l;
	connectorListener = NULL;
}
void HttpConnector::init(ConnectorListener &l)
{
	registerIt(&l);
}
void HttpConnector::publishAccepting(ConnectorEvent e)
{
	//	std::list<ConnectorListener>::iterator ite;
	//	for (ite = connectorListenerList.begin();
	//			ite != connectorListenerList.end(); ite++)
	//	{
	//		ite->onIncomming(e);
	//	}
	connectorListener->onIncomming(e);
}
void HttpConnector::publishDataReceiving(ConnectorEvent e)
{
	//	std::list<ConnectorListener>::iterator iteListeners;
	//	for (iteListeners = connectorListenerList.begin();
	//			iteListeners != connectorListenerList.end(); iteListeners++)
	//	{
	//		iteListeners->onDataReceiving(e);
	//	}
	connectorListener->onDataReceiving(e);
}

HttpConnector::HttpConnector(std::string ipStr, int port, Config *c) : harl(), ns(), _soListen(-1), connectorListener(NULL), fdTab(), fdTabSize(0)
{

	int on = 1;
	struct sockaddr_in sockaddrStruct;

	config = c;

	//	connectorListener = NULL;

	// protocol ip==0 (/etc/protocols)
	harl.trace2("---HttpConnector::HttpConnector: socket");
	if ((_soListen = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		harl.error("HttpConnector::HttpConnector: Creating socket, bye !");
		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}
	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	harl.trace2("---HttpConnector::HttpConnector: setsockopt");
	int rc = setsockopt(_soListen, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
	if (rc < 0)
	{
		harl.error("HttpConnector::HttpConnector : setsockopt() failed");
		close(_soListen);
		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}
	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	harl.trace2("---HttpConnector::HttpConnector: ioctl");
	rc = ioctl(_soListen, FIONBIO, (char*) &on);
	if (rc < 0)
	{
		harl.error("HttpConnector::HttpConnector : ioctl() failed");
		close(_soListen);
		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}

	int option_value = 1;
	harl.trace2("---HttpConnector::HttpConnector: setsockopt");
	setsockopt(_soListen, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	sockaddrStruct.sin_family = AF_INET;
	//	memset(&sockaddrStruct, 0, sizeof(SOCKADDR_IN));
	sockaddrStruct.sin_addr.s_addr = inet_addr(ipStr.c_str());
	sockaddrStruct.sin_port = htons(port);
	harl.trace2("---HttpConnector::HttpConnector: bind");
	if (bind(_soListen, (struct sockaddr*) &sockaddrStruct,
			sizeof(sockaddrStruct)) == -1)
	{
		harl.error("HttpConnector::HttpConnector : bind error : [%s], bye !", strerror(errno));
		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}
	else
	{
		//		harl.info("listening on %s:%i", ipStr.c_str(), port);
	}
	//	----------------------------------------------------------------------
	if (_soListen < 0)
	{
		harl.error("HttpConnector::HttpConnector : bind error : [%s], bye !", strerror(errno));

		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}

	ns.ipServer = ipStr;
	ns.portServer = port;
}

void HttpConnector::doListen()
{
	_listen(_soListen, ns);
}

void HttpConnector::_listen(int _soListen, netStruct ns)
{
	int rc;
	int new_sd = -1;
	int end_server = 0;
	int close_conn = 0;
	//
	int timeout;
//	struct pollfd **fdTab;
//	int fdTabSize = 1;
	int fdTabMaxSize = config->getParamInt("connector_pollfd_size", 200);
	struct pollfd *fdTabTemp = new pollfd[fdTabMaxSize]();
	fdTab = &fdTabTemp;

//	fdTab = new fdTab*[200];
//	char **envp = new char*[envMap.size()]
	int current_size = 0, i;

	harl.info("HttpConnector::_listen : Ecoute sur %s:%i", ns.ipServer.c_str(), ns.portServer);

	harl.trace2("---HttpConnector::_listen: listen");
	rc = listen(_soListen, 5);
	if (rc < 0)
	{
		harl.error("HttpConnector::_listen : failed [%s]", strerror(errno));
		close(_soListen);
		// TODO exit n'est pas dans la liste des fonctions autorisées !
		exit(-1);
	}

	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/
	memset(*fdTab, 0, sizeof(fdTab));

	/*************************************************************/
	/* Set up the initial listening socket                        */
	/*************************************************************/
	fdTab[0]->fd = _soListen;
	fdTab[0]->events = POLLIN;
	fdTabSize = 1;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
//	timeout = (3 * 60 * 1000);
	timeout = config->getParamInt("poll_timeout_ms", 3 * 60 * 1000);

	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do
	{
		/***********************************************************/
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		harl.debug("HttpConnector::_listen : Waiting on poll(%i)... fdTabSize=%i", timeout, fdTabSize);
		rc = poll(*fdTab, fdTabSize, timeout);
		if (rc < 0)
		{
			harl.error("HttpConnector::_listen : poll error : [%s]", strerror(errno));
			dumpFdTab(fdTab, fdTabSize);
//			TODO http error?
			break;
		}
		else if (rc == 0)
		{
			dumpFdTab(fdTab, fdTabSize);
			harl.error("HttpConnector::_listen : poll() timed out : [%s]", strerror(errno));
//			break;
//			TODO http error?
			continue;
		}

		/***********************************************************/
		/* One or more descriptors are readable.  Need to          */
		/* determine which ones they are.                          */
		/***********************************************************/
		current_size = fdTabSize;
		for (i = 0; i < current_size; i++)
		{
			struct pollfd *curentPollFd = &(*fdTab)[i];
			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if (curentPollFd->revents == 0)
				continue;

			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/
			if (curentPollFd->revents != POLLIN)
			{
//			TODO http error?

				harl.error("HttpConnector::_listen :  %d Error! revents = %d", curentPollFd->fd,
						curentPollFd->revents);
				//				end_server = 1;
				//				break;
			}
			if (curentPollFd->fd == _soListen)
			{
				/*******************************************************/
				/* Listening descriptor is readable.                   */
				/*******************************************************/
				harl.debug("++++ HttpConnector::_listen :  %d  Listening socket is readable", _soListen);

				/*******************************************************/
				/* Accept all incoming connections that are            */
				/* queued up on the listening socket before we         */
				/* loop back and call poll again.                      */
				/*******************************************************/
				_acceptIncomingCon(new_sd, _soListen, *fdTab, end_server, fdTabSize);
			}

			/*********************************************************/
			/* This is not the listening socket, therefore an        */
			/* existing connection must be readable                  */
			/*********************************************************/

			else
			{
				harl.debug("**** HttpConnector::_listen : %d Descriptor is readable", curentPollFd->fd);
				close_conn = 0;
				/*******************************************************/
				/* Receive all incoming data on this socket            */
				/* before we loop back and call poll again.            */
				/*******************************************************/

				_onDataReceiving(curentPollFd, close_conn);

			} /* End of existing connection is readable             */
		} /* End of loop through pollable descriptors              */

//		/***********************************************************/
//		/* If the compress_array flag was turned on, we need       */
//		/* to squeeze together the array and decrement the number  */
//		/* of file descriptors. We do not need to move back the    */
//		/* events and revents fields because the events will always*/
//		/* be POLLIN in this case, and revents is output.          */
//		/***********************************************************/
//		if (bReorganizeFdTab == 1)
//		{
//			bReorganizeFdTab = 0;
////			reorganiseFdTab(static_cast<pollfd**>(&fdTab), &fdTabSize);
//			reorganiseFdTab(static_cast<pollfd**>(fdTab), &fdTabSize);
//
//		}

	} while (end_server == 0); /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < fdTabSize; i++)
	{
		if ((*fdTab)[i].fd >= 0)
			close((*fdTab)[i].fd);
	}

//		if (nbActiveFd < 0) {
//			harl.error("select : [%s], bye!", strerror(errno));
//			exit(1);
//			continue;
//		} else if (nbActiveFd == 0) {
//			harl.warning("select : Aucun socket prêt...");
////			exit(1);
////			continue;
//		} else {
////	---------------------------------------------------------------------
//			int _soClient = _newClient(_soListen, NS);
//			fdMax = std::max(fdMax, _soClient);
////			if (!FD_ISSET(_soListen, &NS.readingBitsetOfDescriptors)) {
//			_newClient(_soListen, NS);
////			}
//			_process_ready_for_read(8080, _soListen, NS);
////			_process_ready_for_write();
//		}
//	}
}

void HttpConnector::reorganiseFdTab(pollfd **fdTab, int *fdTabSize)
{
	for (int i = 0; i < *fdTabSize; i++)
	{
		if ((*fdTab)[i].fd == -1)
		{
			for (int j = i; j < *fdTabSize; j++)
			{
				if (j < (*fdTabSize) - 1)
					(*fdTab)[j].fd = (*fdTab)[j + 1].fd;
			}
			i--;
			(*fdTabSize)--;
		}
	}
}

void HttpConnector::dumpFdTab(pollfd **fdTab, int fdTabSize)
{

	harl.trace("HttpConnector::dumpFdTab: fdTabSize=%i", fdTabSize);
	for (int i = 0; i < fdTabSize; i++)
	{
		harl.trace("fdTab[%i] = %i", i, (*fdTab)[i]);
	}

}

void HttpConnector::_acceptIncomingCon(int new_sd, int &_soListen,
		struct pollfd fdTab[], int &end_server, int &nfds)
{
	/*******************************************************/
	/* Accept all incoming connections that are            */
	/* queued up on the listening socket before we         */
	/* loop back and call poll again.                      */
	/*******************************************************/
	do
	{
		/*****************************************************/
		/* Accept each incoming connection. If               */
		/* accept fails with EWOULDBLOCK, then we            */
		/* have accepted all of them. Any other              */
		/* failure on accept will cause us to end the        */
		/* server.                                           */
		/*****************************************************/
		harl.trace2("---HttpConnector::_acceptIncomingCon: accept");
//		sockaddr_in client_addr;
//		socklen_t client_addr_len = sizeof(client_addr);
//		new_sd = accept(_soListen, (struct sockaddr*) &client_addr, &client_addr_len);
		new_sd = accept(_soListen, 0, 0);
//		char str[INET_ADDRSTRLEN];
//		inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
		if (new_sd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				harl.error("HttpConnector::_acceptIncomingCon : accept() failed");
				end_server = 1;
			}
			break;
		}
		harl.trace2("---HttpConnector::_acceptIncomingCon: fcntl");
		if (fcntl(new_sd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		{
//			TODO http error?
			harl.error("HttpConnector::_acceptIncomingCon : fcntl() failed");
			close(new_sd);
			break;
		}
		/*****************************************************/
		/* Add the new incoming connection to the            */
		/* pollfd structure                                  */
		/*****************************************************/
		harl.debug("---- %d HttpConnector::_acceptIncomingCon : New incoming connection", new_sd);
		fdTab[nfds].fd = new_sd;
		fdTab[nfds].events = POLLIN;
		nfds++;
		/*****************************************************/
		/* Loop back up and accept another incoming          */
		/* connection                                        */
		/*****************************************************/
		ConnectorEvent e("Accepting");
		e.setFdClient(&new_sd);
		publishAccepting(e);
	} while (new_sd != -1);
}

bool HttpConnector::_onDataReceiving(struct pollfd *curentPollFd,
		int &close_conn)
{
	bool compress_array = 0;
	int rcv_buffer_size_bytes = config->getParamInt("rcv_buffer_size_bytes", 5000000);
	rcv_buffer_size_bytes *= sizeof(char);
	/*******************************************************/
	/* Receive all incoming data on this socket            */
	/* before we loop back and call poll again.            */
	/*******************************************************/
	char *buffer = new char[rcv_buffer_size_bytes + 1];
	do
	{
		if (curentPollFd->fd == -1)
			break;
		/*****************************************************/
		/* Receive data on this connection until the         */
		/* recv fails with EWOULDBLOCK. If any other         */
		/* failure occurs, we will close the                 */
		/* connection.                                       */
		/*****************************************************/
		harl.trace2("---HttpConnector::_onDataReceiving: recv");
//		char **bufferPtr = &buffer;
		for (int i = 0; i < rcv_buffer_size_bytes; i++)
			buffer[i] = 0;
//		std::memset(buffer, 0, rcv_buffer_size_bytes);
		ssize_t rc = recv(curentPollFd->fd, buffer, rcv_buffer_size_bytes, 0);
		buffer[rcv_buffer_size_bytes] = 0;

		harl.trace2("---HttpConnector::_onDataReceiving: retour recv rc=%i", rc);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				//			TODO http error?
				harl.error(" %d HttpConnector::_onDataReceiving : recv() failed [%s]", curentPollFd->fd,
						strerror(errno));
				close_conn = 1;
			}
			close_conn = 1;
			break;
		}
		else if (rc == 0)
		{
			//			TODO http error?
			harl.error("rc == 0 %d HttpConnector::_onDataReceiving : //Connection closed [%s]", curentPollFd->fd,
					strerror(errno));
			close_conn = 1;
			break;
		}
		else
		{
			/*****************************************************/
			/* Data was received                                 */
			/*****************************************************/
			int len = rc;
			harl.debug(" %d HttpConnector::_onDataReceiving : %d bytes received", curentPollFd->fd, len);
			//					std::cout << buffer << std::endl;
			//					/*****************************************************/
			//					/* Echo the data back to the client                  */
			//					/*****************************************************/
			//			rc = send(curentPollFd.fd, buffer, len, 0);
			//			if (rc < 0) {
			//				harl.error("  send() failed");
			//				close_conn = 1;
			//				break;
			//			}
			std::string cont = std::string(buffer);
			harl.trace(cont);
			ConnectorEvent e(cont);
			e.setFdClient(&curentPollFd->fd);
			publishDataReceiving(e);
			//			close_conn = 1;
			//			break;
		}
//		delete[] buffer;
	} while (1);
	delete[] buffer;

	/*******************************************************/
	/* If the close_conn flag was turned on, we need       */
	/* to clean up this active connection. This            */
	/* clean up process includes removing the              */
	/* descriptor.                                         */
	/*******************************************************/
	if (close_conn)
	{
		harl.trace2("---HttpConnector::_onDataReceiving: closeConnection");
		closeConnection(&curentPollFd->fd);
		compress_array = 1;
	}
	return compress_array;
}

void HttpConnector::closeConnection(int *fd)
{
	harl.debug("---- %i Fermeture de la connexion \n", *fd);
	close(*fd);
	*fd = -1;
	/***********************************************************/
	/* If the compress_array flag was turned on, we need       */
	/* to squeeze together the array and decrement the number  */
	/* of file descriptors. We do not need to move back the    */
	/* events and revents fields because the events will always*/
	/* be POLLIN in this case, and revents is output.          */
	/***********************************************************/
//			reorganiseFdTab(static_cast<pollfd**>(&fdTab), &fdTabSize);
	reorganiseFdTab(static_cast<pollfd**>(fdTab), &fdTabSize);

}
