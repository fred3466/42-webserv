#include "ServerRunner.h"

ServerRunner::ServerRunner() : _pollFds(), nbPollFds(0), configVector(), httpServerList(), fdPollfdServerTuple_List()
{
//	memset(*_pollFds, 0, POLLFD_LIMIT);
}

ServerRunner::~ServerRunner()
{
}

void ServerRunner::run(std::vector<Config*> configVector)
{
	this->configVector = configVector;
	bool bServerListening = true;

	for (std::vector<Config*>::iterator ite = configVector.begin(); ite != configVector.end(); ite++)
	{
//			TODO lancer tous les serveurs, et pas seulement le premier
		HttpServer *server = new HttpServer();
		httpServerList.push_back(server);
		Config *conf = *ite;
		server->init(conf);
		pollfd *firstPollfd = server->getConnector()->getPollFd();
		_pollFds[nbPollFds++] = firstPollfd;

		FdPollfdServerTuple *fdPollfdServerTuple = new FdPollfdServerTuple(firstPollfd->fd, firstPollfd, server, bServerListening);
		fdPollfdServerTuple_List.push_back(fdPollfdServerTuple);

//				struct pollfd **fdTab=
	}

	doPoll();

//			for (std::list<HttpServer*>::iterator ite = httpServerList.begin(); ite != httpServerList.end(); ite++)
//			{
//				HttpServer *server = *ite;
//				Connector *con = server->getConnector();
//				con->doPoll();
//			}
}

void ServerRunner::doPoll()
{
	int rc;
//	int new_sd = -1;
	int end_server = 0;
	int close_conn = 0;
//
	int timeout;
//	struct pollfd **fdTab;
	int fdTabSize = fdPollfdServerTuple_List.size();
//	int fdTabMaxSize = 1024;	//config->getParamInt("connector_pollfd_size", 200);
//	struct pollfd *fdTabTemp = new pollfd[fdTabMaxSize]();
//	fdTab = &fdTabTemp;
	Harl harl;

//	fdTab = new fdTab*[200];
//	char **envp = new char*[envMap.size()]
//	int current_size = 0, i;

//	harl.info("ServerRunner::_listen : Ecoute sur %s:%i", ns.ipServer.c_str(), ns.portServer);

//	harl.trace2("---ServerRunner::_listen: listen");
//	rc = listen(_soListen, 5);
//	if (rc < 0)
//	{
//		harl.error("ServerRunner::_listen : failed [%s]", strerror(errno));
//		close(_soListen);
//		// TODO exit n'est pas dans la liste des fonctions autorisées !
//		exit(-1);
//	}
//
//	/*************************************************************/
//	/* Initialize the pollfd structure                           */
//	/*************************************************************/
//	memset(*fdTab, 0, sizeof(fdTab));
//
//	/*************************************************************/
//	/* Set up the initial listening socket                        */
//	/*************************************************************/
//	fdTab[0]->fd = _soListen;
//	fdTab[0]->events = POLLIN;
//	fdTabSize = 1;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
//	timeout = (3 * 60 * 1000);
//	timeout = config->getParamInt("poll_timeout_ms", 3 * 60 * 1000);
	timeout = 3 * 60 * 1000;
//	timeout = 0;
	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do
	{
		/***********************************************************/
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		harl.debug("ServerRunner::_listen : Waiting on poll(%i)... fdTabSize=%i", timeout, fdTabSize);
		rc = poll(*_pollFds, fdTabSize, timeout);
		if (rc < 0)
		{
			harl.error("ServerRunner::_listen : poll error : [%s]", strerror(errno));
//			dumpFdTab(fdTab, fdTabSize);
			//			TODO http error?
			break;
		}
		else if (rc == 0)
		{
//			dumpFdTab(fdTab, fdTabSize);
			harl.error("ServerRunner::_listen : poll() timed out : [%s]", strerror(errno));
			//			break;
			//			TODO http error?
			continue;
		}

		/***********************************************************/
		/* One or more descriptors are readable.  Need to          */
		/* determine which ones they are.                          */
		/***********************************************************/
//		current_size = fdTabSize;
//		for (i = 0; i < current_size; i++)
		for (std::list<FdPollfdServerTuple*>::iterator ite = fdPollfdServerTuple_List.begin(); ite != fdPollfdServerTuple_List.end(); ite++)
		{
//			struct pollfd *curentPollFd = &(*_pollFds)[i];
			FdPollfdServerTuple *fdPollfdServerTuple = *ite;
			struct pollfd *curentPollFd_PTR = fdPollfdServerTuple->getPollfd();
			struct pollfd curentPollFd = *curentPollFd_PTR;
//			HttpServer *server = fdPollfdServerTuple->getHttpServer();

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
			if (curentPollFd.revents != POLLIN)
			{
				//			TODO http error?

				harl.error("ServerRunner::_listen :  %d Error! revents = %d", curentPollFd.fd,
						curentPollFd.revents);
				//				end_server = 1;
				//				break;
			}
//			if (curentPollFd->fd == _soListen)
			if (fdPollfdServerTuple->isBServerListening())
			{
				/*******************************************************/
				/* Listening descriptor is readable.                   */
				/*******************************************************/
				harl.debug("++++ ServerRunner::_listen :  %d  Listening socket is readable", curentPollFd.fd);

				/*******************************************************/
				/* Accept all incoming connections that are            */
				/* queued up on the listening socket before we         */
				/* loop back and call poll again.                      */
				/*******************************************************/

//				TODO fred mai
				_acceptIncomingCon(fdPollfdServerTuple);
				FdPollfdServerTuple *fdPollfdServerTuple_NEW = fdPollfdServerTuple_List.back();
				_onDataReceiving(fdPollfdServerTuple_NEW, close_conn);
			}

			/*********************************************************/
			/* This is not the listening socket, therefore an        */
			/* existing connection must be readable                  */
			/*********************************************************/

			else
			{
				harl.debug("**** ServerRunner::_listen : %d Descriptor is readable", curentPollFd.fd);
				close_conn = 0;
//				TODO fred mai : supprimer
//				(void) close_conn;

				/*******************************************************/
				/* Receive all incoming data on this socket            */
				/* before we loop back and call poll again.            */
				/*******************************************************/
//				TODO fred mai
				_onDataReceiving(fdPollfdServerTuple, close_conn);
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
//	for (i = 0; i < fdTabSize; i++)
//	{
//		if ((*fdTab)[i].fd >= 0)
//			close((*fdTab)[i].fd);
//	}
}

void ServerRunner::_acceptIncomingCon(FdPollfdServerTuple *fdPollfdServerTuple)
{
	/*******************************************************/
	/* Accept all incoming connections that are            */
	/* queued up on the listening socket before we         */
	/* loop back and call poll again.                      */
	/*******************************************************/
	Harl harl;
	HttpServer *server = fdPollfdServerTuple->getHttpServer();
	int _soListen = fdPollfdServerTuple->getFd();
	int new_sd = -1;
//	bool end_server = 0;

	do
	{
		/*****************************************************/
		/* Accept each incoming connection. If               */
		/* accept fails with EWOULDBLOCK, then we            */
		/* have accepted all of them. Any other              */
		/* failure on accept will cause us to end the        */
		/* server.                                           */
		/*****************************************************/
		harl.trace2("---ServerRunner::_acceptIncomingCon: accept");
		new_sd = accept(_soListen, 0, 0);
		if (new_sd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				harl.error("ServerRunner::_acceptIncomingCon : accept() failed");
//				end_server = 1;
			}
			break;
		}
		harl.trace2("---ServerRunner::_acceptIncomingCon: fcntl");
		if (fcntl(new_sd, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
		{
			//			TODO http error?
			harl.error("ServerRunner::_acceptIncomingCon : fcntl() failed");
			close(new_sd);
			break;
		}
		/*****************************************************/
		/* Add the new incoming connection to the            */
		/* pollfd structure                                  */
		/*****************************************************/
		harl.debug("---- %d ServerRunner::_acceptIncomingCon : New incoming connection", new_sd);

		pollfd *_pollfd = new pollfd();
		_pollfd->fd = new_sd;
		_pollfd->events = POLLIN;
		(*_pollFds)[nbPollFds++] = *_pollfd;

		FdPollfdServerTuple *fdPollfdServerTupleNew = new FdPollfdServerTuple(new_sd, _pollfd, server, false);
		fdPollfdServerTuple_List.push_back(fdPollfdServerTupleNew);
		/*****************************************************/
		/* Loop back up and accept another incoming          */
		/* connection                                        */
		/*****************************************************/
		ConnectorEvent e = ConnectorEvent("Accepting");
		e.setByteBuffer(NULL);
		e.setFdClient(&new_sd);
		e.setFdPollfdServerTuple(fdPollfdServerTupleNew);

		server->onIncomming(&e);
//		publishAccepting(&e);
	} while (new_sd != -1);
}

bool ServerRunner::_onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple, int &close_conn)
{
	Harl harl;
	bool compress_array = 0;
//	int rcv_buffer_size_bytes = config->getParamInt("rcv_buffer_size_bytes", 5000000);
	int rcv_buffer_size_bytes = 5000000;
	rcv_buffer_size_bytes *= sizeof(char);
	/*******************************************************/
	/* Receive all incoming data on this socket            */
	/* before we loop back and call poll again.            */
	/*******************************************************/
	struct pollfd *curentPollFd_PTR = fdPollfdServerTuple->getPollfd();
	struct pollfd curentPollFd = *curentPollFd_PTR;
	HttpServer *server = fdPollfdServerTuple->getHttpServer();

	char *buffer = new char[rcv_buffer_size_bytes + 1];
	do
	{
		if (curentPollFd.fd == -1)
			break;
		/*****************************************************/
		/* Receive data on this connection until the         */
		/* recv fails with EWOULDBLOCK. If any other         */
		/* failure occurs, we will close the                 */
		/* connection.                                       */
		/*****************************************************/
		harl.trace2("---ServerRunner::_onDataReceiving: recv");
		//		char **bufferPtr = &buffer;
		//for (int i = 0; i < rcv_buffer_size_bytes; i++)
		//	buffer[i] = 0;
		std::memset(buffer, 0, rcv_buffer_size_bytes);
		ssize_t rc = recv(curentPollFd.fd, buffer, rcv_buffer_size_bytes, 0);
		buffer[rcv_buffer_size_bytes] = 0;

		harl.trace2("---ServerRunner::_onDataReceiving: retour recv rc=%i", rc);
		if ((rc < 0))
		{
			if (errno != EWOULDBLOCK)
			{
				//			TODO http error?
				harl.error(" %d ServerRunner::_onDataReceiving : recv() failed [%s]", curentPollFd.fd,
						strerror(errno));
				close_conn = 1;
			}
//			if (rc != -1)
//			{
			close_conn = 1;
			break;
//			}
		}
		else if (rc == 0)
		{
			//			TODO http error?
			harl.error("rc == 0 %d ServerRunner::_onDataReceiving : //Connection closed [%s]", curentPollFd.fd,
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
			if (rc <= 0)
				return false;
			harl.debug(" %d ServerRunner::_onDataReceiving : %d bytes received", curentPollFd.fd, len);
			std::string cont = std::string(buffer);
			harl.trace(cont);

			ConnectorEvent e = ConnectorEvent("DataReceiving");
			e.setFdClient(&curentPollFd.fd);
			e.setLen(len);
			char *dataReceived = new char[len + 1]();
			memcpy(dataReceived, buffer, len);
			e.setByteBuffer(dataReceived);
//			publishDataReceiving(&e);
			server->onDataReceiving(&e);
		}
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
		harl.debug("---ServerRunner::_onDataReceiving: closeConnection");
		closeConnection(fdPollfdServerTuple);
		compress_array = 1;
	}
	return compress_array;
}

void ServerRunner::closeConnection(FdPollfdServerTuple *fdPollfdServerTuple)
{
	Harl harl;
	pollfd *_pollfdParam = fdPollfdServerTuple->getPollfd();
	int fd = _pollfdParam->fd;

	harl.debug("---- %i Fermeture de la connexion \n", fd);
	close(fd);

	for (int i = 0; i < nbPollFds; ++i)
	{
		pollfd *pfd = _pollFds[i];
		if (pfd && pfd->fd == fd)
		{
			_pollFds[i]->fd = -1;
		}
	}

	/***********************************************************/
	/* If the compress_array flag was turned on, we need       */
	/* to squeeze together the array and decrement the number  */
	/* of file descriptors. We do not need to move back the    */
	/* events and revents fields because the events will always*/
	/* be POLLIN in this case, and revents is output.          */
	/***********************************************************/
	reorganiseFdTab();
}

void ServerRunner::removeTupleForFd(int fd)
{

	for (std::list<FdPollfdServerTuple*>::iterator ite = fdPollfdServerTuple_List.begin(); ite != fdPollfdServerTuple_List.end(); ite++)
	{
		FdPollfdServerTuple *fdPollfdServerTuple = *ite;
		if (fd == fdPollfdServerTuple->getFd())
		{
			delete fdPollfdServerTuple;
			fdPollfdServerTuple_List.erase(ite);
		}
	}
}

void ServerRunner::reorganiseFdTab()
{
	for (int i = 0; i < nbPollFds; i++)
	{
		if ((*_pollFds)[i].fd == -1)
		{
			for (int j = i; j < nbPollFds; j++)
			{
				if (j < (nbPollFds) - 1)
					(*_pollFds)[j].fd = (*_pollFds)[j + 1].fd;
			}
			i--;
			(nbPollFds)--;
		}
	}
}
