#include "PollManager.h"
#include "processor/CGI/CGIHelper.h"
#include <unistd.h>

bool PollManager::_SIGINT = false;

struct pollfd PollManager::_pollFds[POLLFD_LIMIT];
int PollManager::nbPollFds = 0;
std::list<FdPollfdServerTuple*> PollManager::fdPollfdServerTuple_List;
Harl PollManager::harl;

PollManager::PollManager() {
}

PollManager::~PollManager() {

}

void PollManager::pollFdsInit() {
	for (int index = 0; index < POLLFD_LIMIT; index++)
		pollFdsReset(index);
}

void PollManager::pollFdsReset(const int &index) {
	_pollFds[index].fd = -1;
	_pollFds[index].events = 0;
	_pollFds[index].revents = 0;
}

void PollManager::doPoll(bool bOnlyPipes) {
	int nbPollFdActivated;
	bool endPollingLoop = bOnlyPipes;
	int timeout;

	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = 3 * 60 * 1000;
	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do {
		/***********************************************************/
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		harl.debug("PollManager::doPoll : Waiting on poll(%i)... nbPollFds=%i bOnlyPipes=%i", timeout, nbPollFds, bOnlyPipes);
		if (HARL_LEVEL == -1 || HARL_LEVEL >= 4)
			dumpPollFds("AVANT poll()");

		nbPollFdActivated = poll(_pollFds, nbPollFds, timeout);

		if (HARL_LEVEL == -1 || HARL_LEVEL >= 4)
			dumpPollFds("APRES poll()");

		harl.trace("PollManager::doPoll : poll nb pollFds actifs rc=%i : [%s]", nbPollFdActivated, strerror(errno));
		if (nbPollFdActivated < 0) {
			harl.warning("PollManager::doPoll : poll error : [%s]", strerror(errno));
			harl.debug("PollManager::doPoll : rc=%i errno=%i %s", nbPollFdActivated, errno, strerror(errno));

			if (errno == EINTR) {
				if (ServerRunner::_SIGINT) {
					harl.debug("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOoo PollManager::doPoll : ServerRunner::_SIGINT => break");
					break;
				}
			}
			harl.debug("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOoo PollManager::doPoll : PAS ServerRunner::_SIGINT => continue");
			continue;
		} else if (nbPollFdActivated == 0) {
			continue;
		}

		/***********************************************************/
		/* One or more descriptors are readable.  Need to          */
		/* determine which ones they are.                          */
		/***********************************************************/
		for (int i = 0; i < nbPollFds; i++) {

			pollfd *currentPollFd = &_pollFds[i];

			FdPollfdServerTuple *fdPollfdServerTuple = selectTupleForFd(currentPollFd->fd);
			if (!fdPollfdServerTuple) {
				harl.error("--- %d PollManager::doPoll fdPollfdServerTuple_NEW est NULL on fd=:  %d", currentPollFd->fd, currentPollFd->fd);
			}

			bool bIsSocket = fdPollfdServerTuple->getTypeEnum() == LISTENING_SOCKET_SERVER_TYPE || fdPollfdServerTuple->getTypeEnum() == COMMON_SOCKET_SERVER_TYPE;

			if (bOnlyPipes && bIsSocket) {
				continue;
			}

			harl.trace("--- %d PollManager::doPoll iterate fd=%d events=%d revents=%d", currentPollFd->fd, currentPollFd->fd, currentPollFd->events, currentPollFd->revents);

			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if (currentPollFd->revents == 0)
				continue;

			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/

			if (currentPollFd->revents & POLLNVAL) {
				harl.debug("--- %d PollManager::doPoll : POLLNVAL %i !!!", currentPollFd->fd, currentPollFd->fd);
				closeAndReleasePollFd(fdPollfdServerTuple->getPollfd());
				continue;
			}

			if (currentPollFd->revents & POLLERR) {
				harl.debug("--- %d PollManager::doPoll : POLLERR %i !!!", currentPollFd->fd, currentPollFd->fd);
				closeAndReleasePollFd(fdPollfdServerTuple->getPollfd());
				break;
			}

			if (currentPollFd->revents & POLLHUP) {
				harl.debug("--- %d PollManager::doPoll : POLLHUP %i !!!", currentPollFd->fd, currentPollFd->fd);
				closeAndReleasePollFd(fdPollfdServerTuple->getPollfd());
				continue;
			}

			if ((currentPollFd->revents & POLLIN)) {
				harl.debug("--- %d PollManager::doPoll : POLLIN! revents = %d", currentPollFd->fd, currentPollFd->revents);

				bool bShouldContinue = _readAvailable(fdPollfdServerTuple, *currentPollFd);
				if (fdPollfdServerTuple->isBTooBigError()) {
					harl.debug("--- %d PollManager::doPoll : ServerRunner::sendErrorBodyTooBig for fd=%i", currentPollFd->fd);
					ServerRunner::sendErrorBodyTooBig(fdPollfdServerTuple);
					currentPollFd->events &= ~POLLIN;
					currentPollFd->revents &= ~POLLIN;
					continue;
				}
				if (bIsSocket && fdPollfdServerTuple->getHttpServer() != NULL) {
					currentPollFd->revents &= ~POLLIN;
					currentPollFd->events = (1 | POLLOUT);
				}
				if (bShouldContinue) {
					continue;
				}
			}
			if ((currentPollFd->revents & POLLOUT)) {
				harl.debug("--- %d PollManager::doPoll : POLLOUT! revents = %d", currentPollFd->fd, currentPollFd->revents);

				bool bShouldContinue = _writeAvailable(fdPollfdServerTuple, *currentPollFd);
				if (bShouldContinue) {
					i--;
					continue;
				} else {
					if (bIsSocket && fdPollfdServerTuple /*&& (fdPollfdServerTuple->getTypeEnum() != PIPE_TYPE)*/)
						currentPollFd->revents &= ~POLLOUT;
				}

			}

			if (fdPollfdServerTuple && currentPollFd && (currentPollFd->revents != 0)) {
				harl.debug("--- %d PollManager::doPoll : revents!=0 ==%i, on recommence l'iteration", currentPollFd->fd, currentPollFd->revents);
				i--;
				continue;
			}
			if (!fdPollfdServerTuple) {
				harl.error("--- %d PollManager::doPoll fdPollfdServerTuple_NEW est NULL on fd=:  %d", currentPollFd->fd, currentPollFd->fd);
			}

		} /* End of loop through pollable descriptors              */

	} while (!endPollingLoop); /* End of serving running.    */

}

//Renvoie true si la connexion a été fermée, donc "reorganiseFdTab()" a été appelé,
//et il faut en tenir compte dans le code appelant (boucle for)
bool PollManager::_writeAvailable(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd) {
	bool close_conn = false;
	if (fdPollfdServerTuple->getTypeEnum() == PIPE_TYPE) {
		harl.debug("++++ %d PollManager::_writeAvailable : fdPollfdServerTuple->getTypeEnum() == PIPE_TYPE");
		close_conn = CGIHelper::_onWritingEnabled(fdPollfdServerTuple);
	} else {
		close_conn = ServerRunner::_onWritingEnabled(fdPollfdServerTuple, currentPollFd);
		harl.debug("++++ %d PollManager::_writeAvailable : fdPollfdServerTuple->getTypeEnum() != PIPE_TYPE");
	}
	return close_conn;

}

//renvoie true si la lecture est correcte, pour passer à l'itération suivante dans le for appelant.
bool PollManager::_readAvailable(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd) {
	if (fdPollfdServerTuple && fdPollfdServerTuple->getTypeEnum() == LISTENING_SOCKET_SERVER_TYPE) {
		/*******************************************************/
		/* Listening descriptor is readable.                   */
		/*******************************************************/
		harl.trace("++++ %d PollManager::_readAvailable :   Lecture sur socket d'écoute du serveur", currentPollFd.fd);

		/*******************************************************/
		/* Accept all incoming connections that are            */
		/* queued up on the listening socket before we         */
		/* loop back and call poll again.                      */
		/*******************************************************/

		harl.trace("--- %d PollManager::_readAvailable ACCEPTING on fd=:  %d", currentPollFd.fd, currentPollFd.fd);
		if (HARL_LEVEL == -1 || HARL_LEVEL >= 4)
			dumpPollFds("ACCEPTING:");

		FdPollfdServerTuple *fdPollfdServerTuple_NEW = acceptIncomingCon(fdPollfdServerTuple);
		if (!fdPollfdServerTuple_NEW) {
			harl.error("--- %d PollManager::_readAvailable fdPollfdServerTuple_NEW est NULL on fd=:  %d\b Pb avec accept?", fdPollfdServerTuple->getFd(), fdPollfdServerTuple->getFd());
//			exit(-1);
			return false;
		}

		if (HARL_LEVEL == -1 || HARL_LEVEL >= 4)
			dumpPollFds("APRES _acceptIncomingCon(" + StringUtil().strFromInt(currentPollFd.fd) + ")");

		harl.trace("--- %d PollManager::_readAvailable RECEIVING on fd=:  %d", fdPollfdServerTuple->getFd(), fdPollfdServerTuple->getFd());
//		bool ret =
		ServerRunner::_onDataReceiving(fdPollfdServerTuple_NEW);
		currentPollFd.revents &= ~POLLIN;
		if (fdPollfdServerTuple_NEW->isBTooBigError())
			return false;

		if (!fdPollfdServerTuple_NEW) {
			harl.error("--- %d PollManager::_readAvailable fdPollfdServerTuple_NEW est NULL on fd=:  %d", fdPollfdServerTuple->getFd(), fdPollfdServerTuple_NEW->getFd());
		}
		if (HARL_LEVEL == -1 || HARL_LEVEL >= 3)
			dumpPollFds("_readAvailable");

//		continue;
		return true;

	}
	else if (!fdPollfdServerTuple) {
		closeAndReleasePollFd(&currentPollFd);
		return true;
	}

	/*********************************************************/
	/* This is not the listening socket, therefore an        */
	/* existing connection must be readable                  */
	/*********************************************************/

	else if (fdPollfdServerTuple->getTypeEnum() == COMMON_SOCKET_SERVER_TYPE) {
		harl.trace("**** %d PollManager::_readAvailable : Socket is readable", currentPollFd.fd);

		/*******************************************************/
		/* Receive all incoming data on this socket            */
		/* before we loop back and call poll again.            */
		/*******************************************************/
		ServerRunner::_onDataReceiving(fdPollfdServerTuple);
		if (fdPollfdServerTuple->isBTooBigError())
			return false;

//		return true;

	} else if (fdPollfdServerTuple->getTypeEnum() == PIPE_TYPE) {
		harl.trace("**** %d PollManager::_readAvailable : PIPE is readable", currentPollFd.fd);
		currentPollFd.revents &= ~POLLIN;
		bool bSomethingHasBeenRead = CGIHelper::_onDataReceiving(fdPollfdServerTuple);
		return bSomethingHasBeenRead;
	}

	return false;
}

FdPollfdServerTuple* PollManager::acceptIncomingCon(FdPollfdServerTuple *fdPollfdServerTuple) {
	/*******************************************************/
	/* Accept all incoming connections that are            */
	/* queued up on the listening socket before we         */
	/* loop back and call poll again.                      */
	/*******************************************************/
	Harl harl;
	Server *server = fdPollfdServerTuple->getHttpServer();
	int _soListen = fdPollfdServerTuple->getFd();
	int new_sd = -1;
	FdPollfdServerTuple *fdPollfdServerTupleNew = NULL;
	/*****************************************************/
	/* Accept each incoming connection. If               */
	/* accept fails with EWOULDBLOCK, then we            */
	/* have accepted all of them. Any other              */
	/* failure on accept will cause us to end the        */
	/* server.                                           */
	/*****************************************************/
	harl.trace("---ServerRunner::_acceptIncomingCon: accept");
	sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	new_sd = accept(_soListen, reinterpret_cast<sockaddr*>(&addr), &addrLen);

	if (new_sd < 0) {
		if (errno != EWOULDBLOCK) {
			harl.error("ServerRunner::_acceptIncomingCon : accept() failed");
			return fdPollfdServerTupleNew;
		}
	}
	harl.trace("---ServerRunner::_acceptIncomingCon: fcntl");
	int res = fcntl(new_sd, F_SETFL, O_NONBLOCK);
	if (res < 0)
			{
		harl.error("ServerRunner::_acceptIncomingCon : fcntl() failed : %s", strerror( errno));
		close(new_sd);
		return fdPollfdServerTupleNew;
	}
	/*****************************************************/
	/* Add the new incoming connection to the            */
	/* pollfd structure                                  */
	/*****************************************************/
	harl.trace("---- %d ServerRunner::_acceptIncomingCon : New incoming connection at %i", new_sd, new_sd);

	bool bServerListening = false;
	bool bWriteModeToo = false;
	fdPollfdServerTupleNew = registerIncomingSocket(new_sd, server, bServerListening, bWriteModeToo);

	return fdPollfdServerTupleNew;
}

void PollManager::clearPollFds(pollfd *pfd) {
	pfd->fd = -1;
	pfd->events = 0;
	pfd->revents = 0;
}

int PollManager::findFreePollFd() {
	for (int i = 0; i < POLLFD_LIMIT; ++i) {
		pollfd pfd = _pollFds[i];
		if (pfd.fd == -1) {
			return i;
		}
	}
	return -1;
}

FdPollfdServerTuple* PollManager::registerIncomingSocket(int new_sd, Server *server, bool bServerListening, bool bWriteModeToo) {
	std::string serverName = server->getConfig()->getParamStr("listen", "???") + ":" + server->getConfig()->getParamStr("port", "???");
	harl.debug("---- %i +++++++++PollManager::registerIncomingSocket: new_sd=%d server=%s bServerListening=%d", new_sd, new_sd, serverName.c_str(), bServerListening);
	int ind = findFreePollFd();
	_pollFds[ind].fd = new_sd;
	_pollFds[ind].events = (1 | POLLIN);
	if (bWriteModeToo) {
		_pollFds[ind].events = (1 | POLLOUT);
	}
	_pollFds[ind].revents = 0;
	nbPollFds++;

	FdPollfdServerTupleTypeEnum typeEnum;
	if (bServerListening)
		typeEnum = LISTENING_SOCKET_SERVER_TYPE;
	else
		typeEnum = COMMON_SOCKET_SERVER_TYPE;

	FdPollfdServerTuple *fdPollfdServerTupleNew = new FdPollfdServerTuple(&_pollFds[ind], server, typeEnum);
	if (fdPollfdServerTupleNew)
		fdPollfdServerTuple_List.push_back(fdPollfdServerTupleNew);
	return fdPollfdServerTupleNew;
}

FdPollfdServerTuple* PollManager::setPollFdWritable(int fd) {
	harl.debug("---- %i _-_-_-_-_-_-_PollManager::setPollFdWritable: fd=%i", fd, fd);
	FdPollfdServerTuple *fdPollfdServerTuple = selectTupleForFd(fd);
	pollfd *pfd = fdPollfdServerTuple->getPollfd();
	pfd->events = (1 | POLLOUT);

	dumpPollFds("PollManager::setPollFdWritable");
	return fdPollfdServerTuple;
}

FdPollfdServerTuple* PollManager::registerPipe(int fdPipe, bool bIsInputType) {
	harl.debug("---- %i +++++++++PollManager::registerPipe: fdPipe=%i bIsInputType=%i", fdPipe, fdPipe, bIsInputType);

	int ind = findFreePollFd();
	_pollFds[ind].fd = fdPipe;
	if (bIsInputType) {
		_pollFds[ind].events = (1 | POLLIN);
	} else {
		_pollFds[ind].events = (1 | POLLOUT);
	}
	_pollFds[ind].revents = 0;
	nbPollFds++;

	FdPollfdServerTupleTypeEnum typeEnum = PIPE_TYPE;

	Server *server = NULL;
	FdPollfdServerTuple *fdPollfdServerTupleNew = new FdPollfdServerTuple(&_pollFds[ind], server, typeEnum);
	if (fdPollfdServerTupleNew)
		fdPollfdServerTuple_List.push_back(fdPollfdServerTupleNew);
	return fdPollfdServerTupleNew;
}

void PollManager::closeAndReleasePollFd(pollfd *_pollfdParam) {
	Harl harl;
	if (!_pollfdParam) {
		return;
	}

//	2406
	bool bPOLLERR = _pollfdParam->revents & POLLERR;
	bool bPOLLHUP = _pollfdParam->revents & POLLHUP;
	bool bPOLLIN = _pollfdParam->revents & POLLIN;
	bool bPOLLOUT = _pollfdParam->revents & POLLOUT;

	if ((!(bPOLLERR || bPOLLHUP)) && (bPOLLIN || bPOLLOUT)) {
//	if (bPOLLIN || bPOLLOUT) {
		harl.debug("---- %i XXXXXXXXXXXXXXXXXXXX PollManager::closeAndReleasePollFd: \nANNULATION Fermeture de la connexion revents=%i \n", _pollfdParam->fd, _pollfdParam->revents);
		return;
	}

	int fd = _pollfdParam->fd;
	if (fd <= 0) {
		return;
	}

	harl.debug("---- %i XXXXXXXXXXXXXXXXXXXX PollManager::closeAndReleasePollFd: Fermeture de la connexion %i", fd, fd);

	close(fd);

	removeTupleForFd(fd);

	for (int i = 0; i < nbPollFds; ++i) {
		pollfd pfd = _pollFds[i];
		if (pfd.fd == fd) {
			_pollFds[i].fd = -1;
		}
	}

	reorganisePollFds();
}

void PollManager::removeTupleForFd(int fd) {

	for (std::list<FdPollfdServerTuple*>::iterator ite = fdPollfdServerTuple_List.begin(); ite != fdPollfdServerTuple_List.end(); ite++) {
		FdPollfdServerTuple *fdPollfdServerTuple = *ite;
		if (fdPollfdServerTuple && (fdPollfdServerTuple->getPollfd()) && (fd == fdPollfdServerTuple->getFd())) {
			fdPollfdServerTuple->setPollfd(NULL);
			fdPollfdServerTuple_List.erase(ite);
			delete fdPollfdServerTuple;
			fdPollfdServerTuple = NULL;
			return;
		}
	}
}

FdPollfdServerTuple* PollManager::selectTupleForFd(int fd) {

	for (std::list<FdPollfdServerTuple*>::iterator ite = fdPollfdServerTuple_List.begin(); ite != fdPollfdServerTuple_List.end(); ite++) {
		FdPollfdServerTuple *fdPollfdServerTuple = *ite;
		if (fd == fdPollfdServerTuple->getFd()) {
			return fdPollfdServerTuple;
		}
	}
	return NULL;
}

void swapPollFd(pollfd **pf1, pollfd **pf2) {
	pollfd *pfTmp = *pf1;
	*pf1 = *pf2;
	*pf2 = pfTmp;
}

void copyPollFd(pollfd &pfSrc, pollfd &pfDest) {
	pfDest.fd = pfSrc.fd;
	pfDest.revents = pfSrc.revents;
	pfDest.events = pfSrc.events;
}
void PollManager::reorganisePollFds() {
	if ((HARL_LEVEL == -1) || (HARL_LEVEL >= 3))
		dumpPollFds("Avant reorganiseFdTab");

	for (int i = 0; i < nbPollFds; i++) {
		pollfd pfd = _pollFds[i];
		if (pfd.fd == -1) {
			for (int j = i; j < nbPollFds; j++) {
				if (j < (nbPollFds) - 1) {

					FdPollfdServerTuple *fdPollfdServerTuple = selectTupleForFd(_pollFds[j + 1].fd);
					copyPollFd(_pollFds[j + 1], _pollFds[j]);
					clearPollFds(&_pollFds[j + 1]);
					fdPollfdServerTuple->setPollfd(&_pollFds[j]);

				}
			}
			i--;
			(nbPollFds)--;
			if (HARL_LEVEL == -1 || HARL_LEVEL >= 3)
				dumpPollFds("PollManager::reorganiseFdTa");
		}
	}
	if ((HARL_LEVEL == -1) || (HARL_LEVEL >= 3))
		dumpPollFds("Après reorganiseFdTab");
}

void PollManager::cleanup() {
	harl.debug("PollManager::~cleanup: delete fdPollfdServerTuple_List : %i", fdPollfdServerTuple_List.size());
	for (std::list<FdPollfdServerTuple*>::iterator ite = fdPollfdServerTuple_List.begin(); ite != fdPollfdServerTuple_List.end(); ite++) {
		FdPollfdServerTuple *fdPollfdServerTuple = *ite;
		delete fdPollfdServerTuple;
	}

	for (int index = 0; index < POLLFD_LIMIT; index++) {
		if (_pollFds[index].fd > 0) {
			shutdown(_pollFds[index].fd, SHUT_RDWR);
			close(_pollFds[index].fd);
		}
	}

}

void PollManager::dumpPollFds(std::string intro) {
	Harl harl;

	harl.trace("///////////////\t%s nbPollFds=%i", intro.c_str(), nbPollFds);
	for (int i = 0; i < nbPollFds; i++) {
		pollfd pfd = _pollFds[i];
		FdPollfdServerTuple *fdPollfdServerTuple = selectTupleForFd(pfd.fd);
		std::string ipPortStr = "";
		if (fdPollfdServerTuple) {
			Server *server = fdPollfdServerTuple->getHttpServer();
			if (server) {
				Connector *con = server->getConnector();
				netStruct ns = con->getInternalNetStruct();
				ipPortStr = ns.ipServer + ":" + StringUtil().strFromInt(ns.portServer);
			} else {
				ipPortStr = "<PIPE>";
			}
			harl.trace("_pollFds[%i].fd = %i event=%i revent=[%i] server=%i type=%i %s", i, pfd.fd, pfd.events, pfd.revents,
					fdPollfdServerTuple->getTypeEnum() == LISTENING_SOCKET_SERVER_TYPE, fdPollfdServerTuple->getTypeEnum(), ipPortStr.c_str());
		} else {
			harl.trace("_pollFds[%i].fd = %i event=%i revent=[%i] fdPollfdServerTuple=NULL", i, pfd.fd, pfd.events, pfd.revents);
		}
	}
	harl.trace("///////////////\n");
}

std::list<FdPollfdServerTuple*>* PollManager::getFdPollfdServerTupleList()
{
	return &fdPollfdServerTuple_List;
}
