#include <string>
#include <list>
#include <unistd.h>
#include "config.h"
#include "ServerRunner.h"
#include "config/ConfigReader.h"
#include "API/Connector.h"
#include "HttpServer.h"
#include "util/FileUtil.h"
#include "Uri/Uri.h"

std::vector<Config*> configVector;
static std::list<HttpServer*> httpServerList;

void signalHandler(int signum)
{
	std::cout << "GoodBye.\n";
//	delete configVector;
	for (std::vector<Config*>::iterator ite = configVector.begin(); ite != configVector.end(); ite++)
	{
		delete *ite;
	}

//	for (std::list<HttpServer*>::iterator ite = httpServerList.begin(); ite != httpServerList.end(); ite++)
//	{
//		HttpServer *server = *ite;
//		Connector *con = server->getConnector();
//
//		struct pollfd **fdTab = con->getFdTab();
//
//		for (int i = 0; i < con->getFdTabSize(); i++)
//		{
//			struct pollfd *curentPollFd = &(*fdTab)[i];
////			close((*fdTab)[i]);
//			int fd = curentPollFd->fd;
//			con->closeConnection(&fd);
//		}
//
//	}
	exit(signum);
}

int main(int ac, char **av)
{
//	system("rm -f DBG/*");

	Harl harl = Harl();
	FileUtil fu = FileUtil();

	// TODO gestion des erreurs

	if (ac == 2)
	{
//		pollfd *_pollFds[POLLFD_LIMIT];

		signal(SIGINT, signalHandler);

		httpServerList = std::list<HttpServer*>();
		std::string path = std::string();
		path.assign(av[1]);
		//				std::string path = "conf/webserv.conf";
		ConfigReader cr = ConfigReader();
		configVector = std::vector<Config*>();
		if (!fu.fileExists(path))
		{
			harl.error("Config file missing for path : [%s]", path.c_str());
			return -1;
		}

		harl.info("Reading config file : [%s]", path.c_str());
		bool bValidated = cr.buildConfigVector(&configVector, path);
		if (bValidated)
		{
			ServerRunner serverRunner = ServerRunner();
			serverRunner.run(configVector);
//			for (std::vector<Config*>::iterator ite = configVector.begin(); ite != configVector.end(); ite++)
//			{
////			TODO lancer tous les serveurs, et pas seulement le premier
//				HttpServer *server = new HttpServer();
//				httpServerList.push_back(server);
//				Config *conf = *ite;
//				server->init(conf);
//				pollfd **pollTab = server->getConnector()->getFdTab();
//				_pollFds[nbServers] = pollTab[0];
//				nbServers++;
////				struct pollfd **fdTab=
//			}
//
////			for (std::list<HttpServer*>::iterator ite = httpServerList.begin(); ite != httpServerList.end(); ite++)
////			{
////				HttpServer *server = *ite;
////				Connector *con = server->getConnector();
////				con->doPoll();
////			}

		}
		else
		{
			harl.error("ERROR");
		}
		//	TODO ne pas oublier de nettoyer le vecteur de config*
	}
	else
	{
		std::cout
		<< "Nombre d'argument incorrect, syntaxe :\n webserv <chemin vers le fichier de configuration>"
				<< std::endl;
	}

	return 0;
}

//void doPoll(int _soListen, Config *config, struct pollfd **fdTab, int fdTabSize)
//{
//	int rc;
//	int new_sd = -1;
//	int end_server = 0;
//	int close_conn = 0;
//	//
//	int timeout;
//	//	struct pollfd **fdTab;
//	//	int fdTabSize = 1;
//	int fdTabMaxSize = config->getParamInt("connector_pollfd_size", 200);
//	struct pollfd *fdTabTemp = new pollfd[fdTabMaxSize]();
//	fdTab = &fdTabTemp;
//	Harl harl;
//
//	//	fdTab = new fdTab*[200];
//	//	char **envp = new char*[envMap.size()]
//	int current_size = 0, i;
//
////	harl.info("HttpConnector::_listen : Ecoute sur %s:%i", ns.ipServer.c_str(), ns.portServer);
//
////	harl.trace2("---HttpConnector::_listen: listen");
////	rc = listen(_soListen, 5);
////	if (rc < 0)
////	{
////		harl.error("HttpConnector::_listen : failed [%s]", strerror(errno));
////		close(_soListen);
////		// TODO exit n'est pas dans la liste des fonctions autorisées !
////		exit(-1);
////	}
////
////	/*************************************************************/
////	/* Initialize the pollfd structure                           */
////	/*************************************************************/
////	memset(*fdTab, 0, sizeof(fdTab));
////
////	/*************************************************************/
////	/* Set up the initial listening socket                        */
////	/*************************************************************/
////	fdTab[0]->fd = _soListen;
////	fdTab[0]->events = POLLIN;
////	fdTabSize = 1;
//	/*************************************************************/
//	/* Initialize the timeout to 3 minutes. If no                */
//	/* activity after 3 minutes this program will end.           */
//	/* timeout value is based on milliseconds.                   */
//	/*************************************************************/
//	//	timeout = (3 * 60 * 1000);
//	timeout = config->getParamInt("poll_timeout_ms", 3 * 60 * 1000);
//	timeout = 0;
//	/*************************************************************/
//	/* Loop waiting for incoming connects or for incoming data   */
//	/* on any of the connected sockets.                          */
//	/*************************************************************/
//	do
//	{
//		/***********************************************************/
//		/* Call poll() and wait 3 minutes for it to complete.      */
//		/***********************************************************/
//		harl.debug("HttpConnector::_listen : Waiting on poll(%i)... fdTabSize=%i", timeout, fdTabSize);
//		rc = poll(*fdTab, fdTabSize, timeout);
//		if (rc < 0)
//		{
//			harl.error("HttpConnector::_listen : poll error : [%s]", strerror(errno));
////			dumpFdTab(fdTab, fdTabSize);
//			//			TODO http error?
//			break;
//		}
//		else if (rc == 0)
//		{
////			dumpFdTab(fdTab, fdTabSize);
//			harl.error("HttpConnector::_listen : poll() timed out : [%s]", strerror(errno));
//			//			break;
//			//			TODO http error?
//			continue;
//		}
//
//		/***********************************************************/
//		/* One or more descriptors are readable.  Need to          */
//		/* determine which ones they are.                          */
//		/***********************************************************/
//		current_size = fdTabSize;
//		for (i = 0; i < current_size; i++)
//		{
//			struct pollfd *curentPollFd = &(*fdTab)[i];
//			/*********************************************************/
//			/* Loop through to find the descriptors that returned    */
//			/* POLLIN and determine whether it's the listening       */
//			/* or the active connection.                             */
//			/*********************************************************/
//			if (curentPollFd->revents == 0)
//				continue;
//
//			/*********************************************************/
//			/* If revents is not POLLIN, it's an unexpected result,  */
//			/* log and end the server.                               */
//			/*********************************************************/
//			if (curentPollFd->revents != POLLIN)
//			{
//				//			TODO http error?
//
//				harl.error("HttpConnector::_listen :  %d Error! revents = %d", curentPollFd->fd,
//						curentPollFd->revents);
//				//				end_server = 1;
//				//				break;
//			}
//			if (curentPollFd->fd == _soListen)
//			{
//				/*******************************************************/
//				/* Listening descriptor is readable.                   */
//				/*******************************************************/
//				harl.debug("++++ HttpConnector::_listen :  %d  Listening socket is readable", _soListen);
//
//				/*******************************************************/
//				/* Accept all incoming connections that are            */
//				/* queued up on the listening socket before we         */
//				/* loop back and call poll again.                      */
//				/*******************************************************/
//
////				TODO fred mai
////				_acceptIncomingCon(new_sd, _soListen, *fdTab, end_server, fdTabSize);
//			}
//
//			/*********************************************************/
//			/* This is not the listening socket, therefore an        */
//			/* existing connection must be readable                  */
//			/*********************************************************/
//
//			else
//			{
//				harl.debug("**** HttpConnector::_listen : %d Descriptor is readable", curentPollFd->fd);
//				close_conn = 0;
//				/*******************************************************/
//				/* Receive all incoming data on this socket            */
//				/* before we loop back and call poll again.            */
//				/*******************************************************/
////				TODO fred mai
////				_onDataReceiving(curentPollFd, close_conn);
//			} /* End of existing connection is readable             */
//		} /* End of loop through pollable descriptors              */
//
//		//		/***********************************************************/
//		//		/* If the compress_array flag was turned on, we need       */
//		//		/* to squeeze together the array and decrement the number  */
//		//		/* of file descriptors. We do not need to move back the    */
//		//		/* events and revents fields because the events will always*/
//		//		/* be POLLIN in this case, and revents is output.          */
//		//		/***********************************************************/
//		//		if (bReorganizeFdTab == 1)
//		//		{
//		//			bReorganizeFdTab = 0;
//		////			reorganiseFdTab(static_cast<pollfd**>(&fdTab), &fdTabSize);
//		//			reorganiseFdTab(static_cast<pollfd**>(fdTab), &fdTabSize);
//		//
//		//		}
//
//	} while (end_server == 0); /* End of serving running.    */
//
//	/*************************************************************/
//	/* Clean up all of the sockets that are open                 */
//	/*************************************************************/
//	for (i = 0; i < fdTabSize; i++)
//	{
//		if ((*fdTab)[i].fd >= 0)
//			close((*fdTab)[i].fd);
//	}
//
//	//		if (nbActiveFd < 0) {
//	//			harl.error("select : [%s], bye!", strerror(errno));
//	//			exit(1);
//	//			continue;
//	//		} else if (nbActiveFd == 0) {
//	//			harl.warning("select : Aucun socket prêt...");
//	////			exit(1);
//	////			continue;
//	//		} else {
//	////	---------------------------------------------------------------------
//	//			int _soClient = _newClient(_soListen, NS);
//	//			fdMax = std::max(fdMax, _soClient);
//	////			if (!FD_ISSET(_soListen, &NS.readingBitsetOfDescriptors)) {
//	//			_newClient(_soListen, NS);
//	////			}
//	//			_process_ready_for_read(8080, _soListen, NS);
//	////			_process_ready_for_write();
//	//		}
//	//	}
//}

