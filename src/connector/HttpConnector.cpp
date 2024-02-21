#include "HttpConnector.h"

HttpConnector::HttpConnector() :
		harl() {

}

HttpConnector::~HttpConnector() {
}

HttpConnector::HttpConnector(const HttpConnector &o) {
	*this = o;
}

HttpConnector& HttpConnector::operator=(const HttpConnector &o) {
	if (this != &o)
		*this = o;
	return *this;
}

//bool HttpConnector::operator==(const HttpConnector &o) {
////	if (o)
//	return this->_soListen == o._soListen;
////	return 0;
//}

void HttpConnector::registerIt(ConnectorListener *l) {
//	connectorListenerList.push_back(l);
	connectorListener = l;
}
void HttpConnector::unregisterIt(ConnectorListener *l) {
//	connectorListenerList.remove(l);
	connectorListener = NULL;
}
void HttpConnector::init(ConnectorListener &l) {
	registerIt(&l);
}
void HttpConnector::publishAccepting(ConnectorEvent e) {
//	std::list<ConnectorListener>::iterator iteListeners;
//	for (iteListeners = connectorListenerList.begin();
//			iteListeners != connectorListenerList.end(); iteListeners++) {
//		iteListeners->onIncomming(e);
	connectorListener->onIncomming(e);
//	}
}
void HttpConnector::publishDataReceiving(ConnectorEvent e) {
//	std::list<ConnectorListener>::iterator iteListeners;
//	for (iteListeners = connectorListenerList.begin();
//			iteListeners != connectorListenerList.end(); iteListeners++) {
//		iteListeners->onDataReceiving(e);
	connectorListener->onDataReceiving(e);
//	}

}

HttpConnector::HttpConnector(std::string ipStr, int port) {

	int on = 1;
	struct sockaddr_in sockaddrStruct;
//protocol ip==0 (/etc/protocols)
	if ((_soListen = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		harl.error("Creating socket, bye !");
//			exit(-1);
	}
	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	int rc = setsockopt(_soListen, SOL_SOCKET, SO_REUSEADDR, (char*) &on,
			sizeof(on));
	if (rc < 0) {
		harl.error("setsockopt() failed");
		close(_soListen);
		exit(-1);
	}
	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/
	rc = ioctl(_soListen, FIONBIO, (char*) &on);
	if (rc < 0) {
		harl.error("ioctl() failed");
		close(_soListen);
		exit(-1);
	}

	int option_value = 1;
	setsockopt(_soListen, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	sockaddrStruct.sin_family = AF_INET;
//	memset(&sockaddrStruct, 0, sizeof(SOCKADDR_IN));
	sockaddrStruct.sin_addr.s_addr = inet_addr(ipStr.c_str());
	sockaddrStruct.sin_port = htons(port);
	if (bind(_soListen, (struct sockaddr*) &sockaddrStruct,
			sizeof(sockaddrStruct)) == -1) {
		harl.error("bind error : [%s], bye !", strerror(errno));
//		exit(-1);
	} else {
//		harl.info("listening on %s:%i", ipStr.c_str(), port);
	}
//	----------------------------------------------------------------------
	if (_soListen < 0) {
		harl.error("bind error : [%s], bye !", strerror(errno));
//		exit(-1);
	}

	ns.ipServer = ipStr;
	ns.portServer = port;

}

void HttpConnector::doListen() {
	_listen(_soListen, ns);
}

void HttpConnector::_listen(int _soListen, netStruct ns) {
	int fdMax;
	int nbActiveFd;

	int len, rc, on = 1;
	int new_sd = -1;
	int desc_ready, end_server = 0, compress_array = 0;
	int close_conn;
//
	struct sockaddr_in addr;
	int timeout;
	struct pollfd fds[200];
	int nfds = 1, current_size = 0, i, j;

	harl.info("Ecoute sur %s:%i", ns.ipServer.c_str(), ns.portServer);

	rc = listen(_soListen, 5);
	if (rc < 0) {
		harl.error("listen() failed [%s]", strerror(errno));
		close(_soListen);
		exit(-1);
	}

	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/
	memset(fds, 0, sizeof(fds));

	/*************************************************************/
	/* Set up the initial listening socket                        */
	/*************************************************************/
	fds[0].fd = _soListen;
	fds[0].events = POLLIN;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (3 * 60 * 1000);

	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do {
		/***********************************************************/
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		harl.debug("Waiting on poll()...%i", _allSockets.size());
		rc = poll(fds, nfds, timeout);
		if (rc < 0) {
			harl.error("poll error : [%s]", strerror(errno));
			break;
		} else if (rc == 0) {
			harl.error("poll() timed out : [%s]", strerror(errno));
			break;
		}

		/***********************************************************/
		/* One or more descriptors are readable.  Need to          */
		/* determine which ones they are.                          */
		/***********************************************************/
		current_size = nfds;
		for (i = 0; i < current_size; i++) {
			struct pollfd curentPollFd = fds[i];
			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if (curentPollFd.revents == 0)
				continue;

			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/
			if (curentPollFd.revents != POLLIN) {
				harl.error("  Error! revents = %d", curentPollFd.revents);
//				end_server = 1;
//				break;

			}
			if (curentPollFd.fd == _soListen) {
				/*******************************************************/
				/* Listening descriptor is readable.                   */
				/*******************************************************/
				harl.debug("  Listening socket is readable");

				/*******************************************************/
				/* Accept all incoming connections that are            */
				/* queued up on the listening socket before we         */
				/* loop back and call poll again.                      */
				/*******************************************************/
				_acceptIncomingCon(new_sd, _soListen, fds, end_server, nfds);
			}

			/*********************************************************/
			/* This is not the listening socket, therefore an        */
			/* existing connection must be readable                  */
			/*********************************************************/

			else {
				harl.debug("  Descriptor %d is readable", curentPollFd.fd);
				close_conn = 0;
				/*******************************************************/
				/* Receive all incoming data on this socket            */
				/* before we loop back and call poll again.            */
				/*******************************************************/

				compress_array = _onDataReceiving(curentPollFd, close_conn);

			} /* End of existing connection is readable             */
		} /* End of loop through pollable descriptors              */

		/***********************************************************/
		/* If the compress_array flag was turned on, we need       */
		/* to squeeze together the array and decrement the number  */
		/* of file descriptors. We do not need to move back the    */
		/* events and revents fields because the events will always*/
		/* be POLLIN in this case, and revents is output.          */
		/***********************************************************/
		if (compress_array) {
			compress_array = 0;
			for (i = 0; i < nfds; i++) {
				if (fds[i].fd == -1) {
					for (j = i; j < nfds; j++) {
						fds[j].fd = fds[j + 1].fd;
					}
					i--;
					nfds--;
				}
			}
		}

	} while (end_server == 0); /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++) {
		if (fds[i].fd >= 0)
			close(fds[i].fd);
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
void HttpConnector::_acceptIncomingCon(int new_sd, int &_soListen,
		struct pollfd fds[], int &end_server, int &nfds) {
	/*******************************************************/
	/* Accept all incoming connections that are            */
	/* queued up on the listening socket before we         */
	/* loop back and call poll again.                      */
	/*******************************************************/
	do {
		/*****************************************************/
		/* Accept each incoming connection. If               */
		/* accept fails with EWOULDBLOCK, then we            */
		/* have accepted all of them. Any other              */
		/* failure on accept will cause us to end the        */
		/* server.                                           */
		/*****************************************************/
		new_sd = accept(_soListen, NULL, NULL);
		if (new_sd < 0) {
			if (errno != EWOULDBLOCK) {
				harl.error("  accept() failed");
				end_server = 1;
			}
			break;
		}
		if (fcntl(new_sd, F_SETFL, O_NONBLOCK) < 0) {
			harl.error("  fcntl() failed");
			close(new_sd);
			break;
		}
		/*****************************************************/
		/* Add the new incoming connection to the            */
		/* pollfd structure                                  */
		/*****************************************************/
		harl.debug("  New incoming connection - %d", new_sd);
		fds[nfds].fd = new_sd;
		fds[nfds].events = POLLIN;
		nfds++;
		_allSockets.push_back(fds[nfds].fd);
		/*****************************************************/
		/* Loop back up and accept another incoming          */
		/* connection                                        */
		/*****************************************************/
		ConnectorEvent e("Accepting");
		publishAccepting(e);
	} while (new_sd != -1);
}

bool HttpConnector::_onDataReceiving(struct pollfd &curentPollFd,
		int &close_conn) {
	char buffer[450];
	bool compress_array = 0;
	/*******************************************************/
	/* Receive all incoming data on this socket            */
	/* before we loop back and call poll again.            */
	/*******************************************************/
	do {
		/*****************************************************/
		/* Receive data on this connection until the         */
		/* recv fails with EWOULDBLOCK. If any other         */
		/* failure occurs, we will close the                 */
		/* connection.                                       */
		/*****************************************************/
		int rc = recv(curentPollFd.fd, buffer, sizeof(buffer), 0);
		if (rc < 0) {
			if (errno != EWOULDBLOCK) {
				harl.error("  recv() failed [%s]", strerror(errno));
				close_conn = 1;
			}
			close_conn = 1;
			break;
		} else if (rc == 0) {
			harl.error("  Connection closed [%s]", strerror(errno));
			close_conn = 1;
			break;
		} else {
			/*****************************************************/
			/* Data was received                                 */
			/*****************************************************/
			int len = rc;
			harl.debug("  %d bytes received", len);
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
			ConnectorEvent e(cont);
			publishDataReceiving(e);
//			close_conn = 1;
//			break;
		}
	} while (1);

	/*******************************************************/
	/* If the close_conn flag was turned on, we need       */
	/* to clean up this active connection. This            */
	/* clean up process includes removing the              */
	/* descriptor.                                         */
	/*******************************************************/
	if (close_conn) {
		harl.debug("Fermeture de %i", curentPollFd.fd);
		close(curentPollFd.fd);
		curentPollFd.fd = -1;
		compress_array = 1;
		_allSockets.remove(curentPollFd.fd);
	}
	return compress_array;
}

