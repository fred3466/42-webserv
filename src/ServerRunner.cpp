#include "ServerRunner.h"
#include <unistd.h>

bool ServerRunner::_SIGINT = false;

std::vector<Config*> ServerRunner::configVector;
std::list<Server*> ServerRunner::httpServerList;
Harl ServerRunner::harl;
bool ServerRunner::bAlreadyDelResped = false;
int ServerRunner::nbResp = 0;

ServerRunner::ServerRunner() {
}

ServerRunner::~ServerRunner() {

}

void ServerRunner::delRequest(Request *request) {
	if (request) {
		Harl().debug(">>>>>>>>>>>>>>>>>>>>>ServerRunner::delRequest fd=%i", request->getFdClient());
		delete request;
		request = NULL;
	} else
		Harl().debug(">>>>>>>>>>>>>>>>>>>>>ServerRunner::delRequest <NULL>");
}

void ServerRunner::delResp(Response *resp) {
//	if (ServerRunner::bAlreadyDelResped) {
	if (ServerRunner::nbResp <= 0) {
		Harl().debug(">>>>>>>>>>>>>>>>>>>>>ServerRunner::delResp ANNULE");
//		ServerRunner::bAlreadyDelResped = false;
		return;
	}
	if (resp) {
		Harl().debug(">>>>>>>>>>>>>>>>>>>>>ServerRunner::delResp ");
		ServerRunner::nbResp--;
		delete resp;
		resp = NULL;
		ServerRunner::bAlreadyDelResped = true;
	} else
		Harl().debug(">>>>>>>>>>>>>>>>>>>>>ServerRunner::delResp <NULL>");
}

void ServerRunner::cleanup() {

	for (std::list<Server*>::iterator ite = httpServerList.begin(); ite != httpServerList.end(); ite++) {
		Server *server = *ite;
		std::string server_name = server->getConfig()->getParamStr("server_name", "UNNAMED SERVER ?");

		harl.debug("ServerRunner::signal_handler: kill server %s", server_name.c_str());
		delete server;
	}
	PollManager::cleanup();
	MimeTypeList::cleanUp();
	ConnectorBuffer::clearAll();
}

void ServerRunner::cleanUpRequestResponse(Request *request, Response *resp) {
	harl.debug("ServerRunner::cleanUpRequestResponse: ");
	delRequest(request);
	delResp(resp);
}

Connector* ServerRunner::isExistingServerConnector(Connector *conParam) {
	for (std::list<Server*>::iterator ite = httpServerList.begin(); ite != httpServerList.end(); ite++) {
		Server *server = *ite;
		Connector *con = server->getConnector();
		netStruct ns = con->getInternalNetStruct();
		netStruct nsParam = conParam->getInternalNetStruct();
		if ((ns.ipServer == nsParam.ipServer) && (ns.portServer == nsParam.portServer)) {
			return con;
		}
	}
	return NULL;
}

void ServerRunner::signal_handler(int signal) {
	if (signal == SIGINT) {
		_SIGINT = true;
	}

//	cleanup();
}

int ServerRunner::run(std::vector<Config*> configVector) {
	PollManager::pollFdsInit();

	Harl harl;

	for (std::vector<Config*>::iterator ite = configVector.begin(); ite != configVector.end(); ite++) {
		Server *server = new HttpServer();

		Config *conf = *ite;
		server->init(conf);

		Connector *con = server->getConnector();
		Connector *oldCon = isExistingServerConnector(con);
		if (oldCon) {
			delete con;
			server->setConnector(oldCon);
			oldCon->registerIt(server);
		} else {
			if (server) {
				int fdServer = con->getSoListen();
				bool bServerListening = true;
				bool bWriteModeToo = true;
				FdPollfdServerTuple *fdPollfdServerTupleNew = PollManager::registerIncomingSocket(fdServer, server, bServerListening, bWriteModeToo);
				pollfd *serverPollFd = fdPollfdServerTupleNew->getPollfd();
				con->setListeningPollFd(serverPollFd);
			}
			con->registerIt(server);
			if (!con->start()) {
				delete server;
				return 0;
			}
		}
		httpServerList.push_back(server);
	}

	signal(SIGINT, ServerRunner::signal_handler);

	bool bOnlyPipes = false;
	PollManager::doPoll(bOnlyPipes);

	return 1;
}

bool ServerRunner::_onWritingEnabled(FdPollfdServerTuple *&fdPollfdServerTuple, pollfd &currentPollFd) {
	int fd = fdPollfdServerTuple->getFd();
	if (fd != currentPollFd.fd) {
		harl.error("ServerRunner::_onWritingEnabled: incohérence fd ! fd=%i currentPollFd.fd=%i", fd, currentPollFd.fd);
	}
	ConnectorBuffer *connectorBuffer_WRITE = ConnectorBuffer::findByFd_WRITE(fd);
	if (!connectorBuffer_WRITE) {
		connectorBuffer_WRITE = new ConnectorBuffer(fd, TYPE_WRITE);
	}
	char *buf = connectorBuffer_WRITE->getBuffer();
	int len = connectorBuffer_WRITE->getLen();

//	if (len) {
//		std::string fname = "DBG/out_" + StringUtil().strFromInt(len) + ".html";
//		std::ofstream os(fname.c_str(), std::ios::binary | std::ios::out);
//		os.write(buf, len);
//		os.close();
//	}

	size_t nbSent = send(fd, buf, len, 0);
	currentPollFd.revents &= ~POLLOUT;

	harl.debug("-_-_-_-_-_-_----%d ServerRunner::_onWritingEnabled: -_-_-_-_-_-_-[ SENT %d / %d BYTES TO THE WIRE ]-_-_-_-_-_-_-\n", fd, nbSent, len);

	Request *request = fdPollfdServerTuple->getRequest();
	Response *resp = fdPollfdServerTuple->getResponse();
	ServerRunner::cleanUpRequestResponse(request, resp);
	fdPollfdServerTuple->setRequest(NULL);
	fdPollfdServerTuple->setResponse(NULL);

	bool close_conn = fdPollfdServerTuple->isBConnectionCloseRequired() || (nbSent == (size_t) 0) || (nbSent == (size_t) -1);
	if (close_conn) {
		PollManager::closeAndReleasePollFd(&currentPollFd);
	}

	if ((nbSent == connectorBuffer_WRITE->getLen()) || (nbSent == (size_t) 0) || (nbSent == (size_t) -1)) {
		ConnectorBuffer::removeFromList_WRITE(connectorBuffer_WRITE);
	}
	return close_conn;
}

void ServerRunner::_onDataReceiving(FdPollfdServerTuple *fdPollfdServerTuple) {

	struct pollfd *curentPollFd_PTR = fdPollfdServerTuple->getPollfd();
	struct pollfd curentPollFd = *curentPollFd_PTR;
	if (fdPollfdServerTuple->isBTooBigError()) {
		if (curentPollFd_PTR) {
			harl.trace("--- %d ServerRunner::_onDataReceiving:  (BEGIN)isBTooBigError=>return", curentPollFd.fd);
//			PollManager::closeAndReleasePollFd(curentPollFd_PTR);
		}
		return;
	}

	Harl harl;
	bool bTooBigError = false;
	int close_conn = false;
	Config *config = fdPollfdServerTuple->getHttpServer()->getConfig();
	int rcv_buffer_size_bytes = config->getParamInt("rcv_buffer_size_bytes", 65 * 1024);
	rcv_buffer_size_bytes *= sizeof(char);
	/*******************************************************/
	/* Receive all incoming data on this socket            */
	/* before we loop back and call poll again.            */
	/*******************************************************/
	size_t maxBodySize = fdPollfdServerTuple->getHttpServer()->getConfig()->getParamInt("max_body_size", 4096);

	Request *request = NULL;
	size_t headerSize = 0;
	size_t max_request_size = config->getParamInt("max_request_size", 64 * 1024);

	ConnectorBuffer *connectorBuffer_READ;
	char *bufferRead = new char[rcv_buffer_size_bytes];
	do {
		if (curentPollFd.fd == -1)
			break;
		/*****************************************************/
		/* Receive data on this connection until the         */
		/* recv fails with EWOULDBLOCK. If any other         */
		/* failure occurs, we will close the                 */
		/* connection.                                       */
		/*****************************************************/
		harl.trace("--- %d ServerRunner::_onDataReceiving: recv", curentPollFd.fd);

		std::memset(bufferRead, 0, rcv_buffer_size_bytes);
		usleep(fdPollfdServerTuple->getHttpServer()->getConfig()->getParamInt("usleep_time_us", 1000));
		ssize_t rc = recv(curentPollFd.fd, bufferRead, rcv_buffer_size_bytes, 0);

		harl.trace("--- %d ServerRunner::_onDataReceiving: retour recv rc=%i", curentPollFd.fd, rc);

		ConnectorBuffer::dumpConBufList_READ();
		connectorBuffer_READ = ConnectorBuffer::findByFd_READ(curentPollFd.fd);

//		Une erreur s'est produite
		if ((rc < 0)) {
			if (connectorBuffer_READ && connectorBuffer_READ->getLen() > 0) {

				request = buildCompleteResponse(connectorBuffer_READ, fdPollfdServerTuple);
				headerSize = request->getHeader()->getHeaderSize();
				//	TODO 2206 deplacer ?
//				cleanUpRequestResponse(request, NULL);

				size_t nbReadUntilNow = connectorBuffer_READ->getLen();
				if (max_request_size < nbReadUntilNow) {
					bTooBigError = true;
					break;
				}
				if ((maxBodySize + headerSize) < nbReadUntilNow) {
					bTooBigError = true;
					break;
				}

				// TODO annuler qqpart !!!
				if (!CGIHelper::incompleteResponse)
					fdPollfdServerTuple->setBConnectionCloseRequired(true);

				ConnectorBuffer::removeFromList_READ(connectorBuffer_READ);
				break;
			} else {
				harl.error(" %d ServerRunner::_onDataReceiving : recv() failed ", curentPollFd.fd);

				harl.trace("XXXXX ServerRunner::_onDataReceiving: CLOSE REQUIRED : rc<0 =%i ", rc);
				close_conn = 1;
				break;
			}
		}

		//		Timeout ou reception normale de 0 octets
		else if (rc == 0) {
			int len = 0;
			if (!connectorBuffer_READ) {
				break;
			}

			len = connectorBuffer_READ->getLen();
			if (len > 0) {
				harl.trace("---%d rc == 0 ServerRunner::_onDataReceiving : //recv rc=0 pour fd=[%d] ", curentPollFd.fd, curentPollFd.fd);

				request = buildCompleteResponse(connectorBuffer_READ, fdPollfdServerTuple);
				headerSize = request->getHeader()->getHeaderSize();
				//	TODO 2206 deplacer ?
//				cleanUpRequestResponse(request, NULL);

				size_t nbReadUntilNow = connectorBuffer_READ->getLen();
				if (max_request_size < (nbReadUntilNow - headerSize)) {
					bTooBigError = true;
					break;
				}
//				headerSize = request->getHeader()->getHeaderSize();
				if ((maxBodySize + headerSize) < nbReadUntilNow) {
					bTooBigError = true;
					break;
				}

				ConnectorBuffer::removeFromList_READ(connectorBuffer_READ);
				break;
			} else {
				harl.trace("---%d rc == 0 ServerRunner::_onDataReceiving : pour fd=[%d] reçu 0 et len buffer=0, timeout ? ", curentPollFd.fd, curentPollFd.fd);
				break;
			}

		} else {
			/*****************************************************/
			/* Data was received                                 */
			/*****************************************************/

			harl.trace(" %d ServerRunner::_onDataReceiving : %d bytes received", curentPollFd.fd, rc);

			if (!connectorBuffer_READ) {
				connectorBuffer_READ = new ConnectorBuffer(curentPollFd.fd, TYPE_READ);
			}
			size_t bufferLen = connectorBuffer_READ->getLen();
			bufferLen += rc;
			size_t nbReadUntilNow = connectorBuffer_READ->getLen();
			if (max_request_size < ((size_t) nbReadUntilNow)) {
				bTooBigError = true;
				break;
			}
			if ((maxBodySize + headerSize) < nbReadUntilNow) {
				bTooBigError = true;
				break;
			}
			connectorBuffer_READ->concatBuffer(bufferRead, rc);
		}
	}
	while (1);
	delete[] bufferRead;

	/*******************************************************/
	/* If the close_conn flag was turned on, we need       */
	/* to clean up this active connection. This            */
	/* clean up process includes removing the              */
	/* descriptor.                                         */
	/*******************************************************/
	if (close_conn) {
		if (curentPollFd_PTR)
			harl.trace("--- %d ServerRunner::_onDataReceiving: closeConnection", curentPollFd.fd);
		PollManager::closeAndReleasePollFd(curentPollFd_PTR);
	}

	if (bTooBigError) {
		ConnectorBuffer::removeFromList_READ(connectorBuffer_READ);
	}

	fdPollfdServerTuple->setBTooBigError(bTooBigError);
}

void ServerRunner::sendErrorBodyTooBig(FdPollfdServerTuple *fdPollfdServerTuple) {
	Response *resp = createErrorResponse(413);
	Request *request = NULL;
	fdPollfdServerTuple->setRequest(request);
	fdPollfdServerTuple->setResponse(resp);

//	int fdClient = fdPollfdServerTuple->getFd();
//	prepareToPushItIntoTheWire(fdClient, request, resp);

	sendResponse(fdPollfdServerTuple, request, resp);
	fdPollfdServerTuple->setBConnectionCloseRequired(true);

//	TODO 2106 ERROR ???
//	delResp(resp);
}

Request* ServerRunner::buildCompleteResponse(ConnectorBuffer *connectorBuffer_READ, FdPollfdServerTuple *fdPollfdServerTuple) {
	Harl harl;
	RequestResponseParam requestResponseParam = RequestResponseParam("DataReceiving");
	requestResponseParam.setFdPollfdServerTuple(fdPollfdServerTuple);
	requestResponseParam.setLen(connectorBuffer_READ->getLen());
	requestResponseParam.setByteBuffer(connectorBuffer_READ->getBuffer());

	requestResponseParam = createRequestInEvent(requestResponseParam);
	Request *request = requestResponseParam.getRequest();
	if (!request)
		return NULL;;
	RequestHeader *reqHeader = request->getHeader();
	Uri uri = reqHeader->getUri();
	int fdSocket = fdPollfdServerTuple->getFd();

	std::string host = reqHeader->getFieldValue("Host");
	std::string unknownHost = "<HOST ?>";
	char *hostC = (char*) unknownHost.c_str();
	if (host != "") {
		hostC = (char*) host.c_str();
	}
	harl.info(
			"\nServerRunner::buildResponse :\n****************************************************************************\n[%s fd=%i]\t%s\n***********************************************************************************************",
			hostC, fdSocket, (reqHeader->getMethod() + " " + uri.getUri()).c_str());
	harl.debug("\nServerRunner::buildResponse :HEADER HTTP:\n%s", reqHeader->toString().c_str());

	Server *server = fdPollfdServerTuple->getHttpServer();
	Connector *con = server->getConnector();

	std::list<ResponseProducer*> responseProducerList = con->getResponseProducer();
	Response *resp = NULL;

	for (std::list<ResponseProducer*>::iterator ite = responseProducerList.begin(); ite != responseProducerList.end(); ite++) {
		ResponseProducer *httpServer = *ite;

//		Le premier serveur qui répond a gagné
		resp = httpServer->createResponse(&requestResponseParam);
		if (resp) {
			resp->setFd(fdSocket);
			fdPollfdServerTuple->setResponse(resp);
			fdPollfdServerTuple->setRequest(request);

//			HttpServer::freeProcessorList();
			break;
		}
	}

	ServerRunner::bAlreadyDelResped = false;
	if (!resp) {
		harl.warning("ServerRunner::buildResponse : No processor for host:port/route [%s]", uri.getUri().c_str());
		resp->setErrorCodeTmp(404);
	} else {

		if (resp && resp->isBNeedFurtherProcessing()) {
			CGIHelper::incompleteResponse = resp;
			CGIHelper::incompleteResponseCorrespondingRequest = request;
		} else {
			sendResponse(fdPollfdServerTuple, request, resp);
		}
	}

	return request;
}

void ServerRunner::sendResponse(FdPollfdServerTuple *fdPollfdServerTuple, Request *request, Response *resp) {
	Harl harl;

	int fdSocket = -1;
	if (resp && (resp->getFd() != -1))
		fdSocket = resp->getFd();
	else
		fdSocket = fdPollfdServerTuple->getFd();

	if (resp && (!resp->isBNeedFurtherProcessing())) {
//		2506
		int errCode = resp->getErrorCodeTmp();
		if ((request && (!request->isResponseAlreadySent())) && (errCode != 200) && (!resp->isRedirect())) {
			harl.debug("ServerRunner::sendResponse : %i Http Error response sent in sendResponse for code [%i] on fd=%i", fdSocket, errCode, fdSocket);
			delResp(resp);
			resp = createErrorResponse(errCode);
			resp->setFd(fdSocket);
			if (fdPollfdServerTuple)
				fdPollfdServerTuple->setResponse(resp);
			resp->setErrorCodeTmp(errCode);
		}

		int nbSent = prepareToPushItIntoTheWire(fdSocket, request, resp);

		if (resp && (!resp->isBNeedFurtherProcessing())) {
			FdPollfdServerTuple *fdPollfdServerTuple = PollManager::selectTupleForFd(fdSocket);

			bool bConnectionKeepAlive = false;
			if (request)
				bConnectionKeepAlive = request->isConnectionKeepAlive();

			if (fdPollfdServerTuple) {
				fdPollfdServerTuple->setBKeepAlive(bConnectionKeepAlive);
				bool bConnectionCloseRequired = fdPollfdServerTuple->isBConnectionCloseRequired();
				harl.debug("ServerRunner::sendResponse :fdPollfdServerTuple->isBConnectionCloseRequired()=%i", bConnectionCloseRequired);

				if (!bConnectionCloseRequired) {
					if (!bConnectionKeepAlive || ((resp->isCgi() || resp->isRedirect()) && nbSent == resp->getTotalLength())) {
						harl.debug("ServerRunner::sendResponse :setBConnectionCloseRequired(true)");
						fdPollfdServerTuple->setBConnectionCloseRequired(true);
					} else {
						harl.debug("ServerRunner::sendResponse :setBConnectionCloseRequired(false)");
						fdPollfdServerTuple->setBConnectionCloseRequired(false);
					}
				}
			}
		}
		ServerRunner::bAlreadyDelResped = false;
		delResp(resp);

	}
	if (request)
		request->setResponseAlreadySent(true);

	PollManager::setPollFdWritable(fdSocket);
}

Response* ServerRunner::createErrorResponse(int errorCode /*, Request *request*/) {
	Harl harl;
	StringUtil su;
	HttpErrorFactory errorFactory;
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);

	HttpError *error = errorFactory.build(errorCode);
	resp->setStatusLine(error->getStatusLine());
	resp->getHeader()->addField("Content-Type", "text/html;");
	resp->setErrorCodeTmp(errorCode);
	if (errorCode != 200) {
		FileUtil fu;
		harl.debug("ServerRunner::createErrorResponse create error page for code=%i", errorCode);
		HttpReturnCodeHelper errHelper = HttpReturnCodeHelper(errorCode);
		char *pageContentArray;

		std::string root_path = "error_pages";

		std::string errorPageFileName = "error_" + su.strFromInt(errorCode) + ".html";
		std::string errorPageFileNameAbs = fu.realPathFile(root_path + "/" + errorPageFileName);
		if (!fu.fileExists(errorPageFileNameAbs)) {
			errorPageFileName = "error.html";
			errorPageFileNameAbs = fu.realPathFile(root_path + "/" + errorPageFileName);
			if (!fu.fileExists(errorPageFileNameAbs)) {
				return resp;
			}
		}

		int readRetCode = FileUtil().readFile(errorPageFileNameAbs, &pageContentArray);
		if (readRetCode == -1) {
			return resp;
		}
		std::string pageContent;				//= std::string(pageContentArray);
		pageContent.assign(pageContentArray, readRetCode);
		delete[] pageContentArray;
		pageContent = su.replace_all(pageContent, "[HTTP_ERROR_CODE]", su.strFromInt(errorCode));
		pageContent = su.replace_all(pageContent, "[HTTP_ERROR_DESC]", error->getDescription());

		char *bodybin = new char[pageContent.length() + 1];
		std::copy(pageContent.begin(), pageContent.end(), bodybin);
		resp->setBodyBin(bodybin);

		int len = pageContent.length();
		resp->setBodyLength(len);
	}

	return resp;
}

int ServerRunner::prepareToPushItIntoTheWire(int fdSocket, Request *request, Response *resp) {
	if (request && request->isResponseAlreadySent())
		return resp->getTotalLength();

	Harl harl;
	char *cstr = packageResponseAndGiveMeSomeBytes(request, resp);
	int length = resp->getTotalLength();

	if (fdSocket >= 0 && cstr && length) {
		ConnectorBuffer *connectorBuffer_WRITE = ConnectorBuffer::findByFd_WRITE(fdSocket);
		if (!connectorBuffer_WRITE) {
			connectorBuffer_WRITE = new ConnectorBuffer(fdSocket, TYPE_WRITE);
		}

		connectorBuffer_WRITE->concatBuffer(cstr, length);
	}
	harl.debug("---%d ServerRunner::prepareToPushItIntoTheWire: added %d bytes to connectorBuffer_WRITE \n%s", fdSocket, length, resp->getHeader()->getStatusLine().c_str());
	std::string cstrStr = std::string(cstr, length);
	harl.trace2("---%d ServerRunner::prepareToPushItIntoTheWire: ready to send [%s]", fdSocket, cstrStr.c_str());
	delete[] cstr;

	if (request)
		request->setResponseAlreadySent(true);

	return length;
}

char* ServerRunner::packageResponseAndGiveMeSomeBytes(Request *request, Response *resp) {
	Harl harl;
	StringUtil stringUtil;

	if (!resp || !resp->getHeader() /*|| resp->getHeader()->getStatusLine().empty()*/) {
		harl.error("ServerRunner::packageResponseAndGiveMeSomeBytes : Problème avec response : \n[%s]", request->getUri().getUri().c_str());
	}

	addUltimateHeaders(resp);
	std::list<std::string> *fields = resp->getHeader()->getFields();
	std::string fieldsString = stringUtil.fromListToString(fields, "") + "\r\n";
	delete fields;

	std::string statusLine = resp->getHeader()->getStatusLine();
	std::string body = "";
	char *bodyBin = resp->getBodyBin();
	if (bodyBin)
		body = std::string(bodyBin, resp->getBodyLength());

	std::string statusHeaderBody = resp->getHeader()->getStatusLine() + fieldsString + body;

	harl.trace("ServerRunner::packageResponseAndGiveMeSomeBytes:  statusLine   : %s", statusLine.c_str());
	harl.trace("ServerRunner::packageResponseAndGiveMeSomeBytes:  fieldsString : %s", fieldsString.c_str());

	int statusLen = statusLine.length();
	int headerLen = fieldsString.length();
	int bodyLen = resp->getBodyLength();

	int length = statusLen + headerLen;
	if ((!request) || (request->getMethod() != "HEAD")) {
		length += bodyLen;
	}
	resp->setTotalLength(length);

	char *cstr = new char[length + 0]();
	char *cstrSave = cstr;
	char **cstrPtr = &cstr;

	std::memcpy(*cstrPtr, statusLine.c_str(), statusLen);
	*cstrPtr += statusLen + 0;
	std::memcpy(*(cstrPtr), fieldsString.c_str(), headerLen);

	if ((!request) || (request->getMethod() != "HEAD")) {
		*cstrPtr += headerLen + 0;
		std::memcpy(*(cstrPtr), resp->getBodyBin(), bodyLen);
	}

	cstr = cstrSave;
//	if ((HARL_LEVEL >= 3) && length)
//			{
//		std::ofstream os("out.txt", std::ios::binary | std::ios::out);
//		os.write(cstr, length);
//		os.close();
//	}
	return cstrSave;
}

void ServerRunner::addUltimateHeaders(Response *resp) {
	Harl harl;
	ResponseHeader *header = resp->getHeader();
//   The Content-Length entity-header field indicates the size of the
//   entity-body, in decimal number of OCTETs, sent to the recipient or,
//   in the case of the HEAD method, the size of the entity-body that
//   would have been sent had the request been a GET.
//
//       Content-Length    = "Content-Length" ":" 1*DIGIT
//
//   An example is
//
//       Content-Length: 3495
//
//   Applications SHOULD use this field to indicate the transfer-length of
//   the message-body, unless this is prohibited by the rules in section
//   4.4.

//	4.4 Message Length
//
//	   The transfer-length of a message is the length of the message-body as
//	   it appears in the message; that is, after any transfer-codings have
//	   been applied. When a message-body is included with a message, the
//	   transfer-length of that body is determined by one of the following
//	   (in order of precedence):
//
//	   1.Any response message which "MUST NOT" include a message-body (such
//	     as the 1xx, 204, and 304 responses and any response to a HEAD
//	     request) is always terminated by the first empty line after the
//	     header fields, regardless of the entity-header fields present in
//	     the message.
//
//	   2.If a Transfer-Encoding header field (section 14.41) is present and
//	     has any value other than "identity", then the transfer-length is
//	     defined by use of the "chunked" transfer-coding (section 3.6),
//	     unless the message is terminated by closing the connection.
//
//	   3.If a Content-Length header field (section 14.13) is present, its
//	     decimal value in OCTETs represents both the entity-length and the
//	     transfer-length. The Content-Length header field MUST NOT be sent
//	     if these two lengths are different (i.e., if a Transfer-Encoding
	std::string transferEncoding = header->getFieldAsStr("Transfer-Encoding", "");
	int contentLengthHeader = header->getFieldAsInt("Content-Length", -1);
	int contentLengthResponse = resp->getBodyLength();

	if (transferEncoding != "" && contentLengthHeader != -1) {
		harl.error("ServerRunner::addUltimateHeaders: présence des entêtes incompatibles\nTransfer-Encoding = [%s]\nContent-Length=[%i]", transferEncoding, contentLengthHeader);
	}
	if (contentLengthHeader != 0 && contentLengthHeader != contentLengthResponse && contentLengthHeader != contentLengthResponse) {
		harl.info(
				"ServerRunner::addUltimateHeaders: Incohérence entre le Content-Length dans l'entête de la Response, et celui renvoyé par Response->getBodyLength():\ncontentLengthHeader=[%i]\ncontentLengthResponse=[%i]",
				contentLengthHeader, contentLengthResponse);
	}
	if (resp->isCgi())
		header->addField("Transfer-Encoding", "chunked");
	else
	if (!resp->isRedirect() && transferEncoding == "" && ((contentLengthHeader == -1) || (contentLengthHeader == 0))) {
		StringUtil su;
		header->addField("Content-Length", su.strFromInt(contentLengthResponse));
	}
}

static int ind = 0;
RequestResponseParam ServerRunner::createRequestInEvent(RequestResponseParam e) {
	Harl harl;
	char *rawRequest = e.getByteBuffer();
	int rawRequestLen = e.getLen();
	int rawRequestBuffer_Length = -1;

	if (HARL_LEVEL >= 4) {
		FileUtil fu = FileUtil();
		StringUtil su = StringUtil();
		std::string fname = "DBG/" + su.strFromInt(ind++) + "_request_frag.txt";
		fu.writeFile(fname.c_str(), rawRequest, rawRequestLen);
	}

//Digère bien l'entête jusqu'à la ligne vide !
	RequestHelper requestHelper = RequestHelper(rawRequest, rawRequestLen);
	if (!requestHelper.bJustInitialized) {
		return RequestResponseParam();
	}

	char *rawRequestBuffer = requestHelper.process(rawRequest, rawRequestLen);
	if (rawRequestBuffer == NULL) {
		delete[] rawRequestBuffer;
		return e;
	} else {
		rawRequestBuffer_Length = requestHelper.getRawRequestBufferLength();

		if (HARL_LEVEL >= 4) {
			FileUtil fu = FileUtil();
			std::string fnameRequest = "DBG/onDataReceiving_defrag_request.txt";
			fu.writeFile(fnameRequest.c_str(), rawRequestBuffer, rawRequestBuffer_Length);
		}

		RequestHeader *reqHeader = RequestHeaderFactory().build(rawRequestBuffer, rawRequestBuffer_Length);

		std::string CONTENT_LENGTH = reqHeader->getFieldValue("Content-Length");
		int bodyLen = StringUtil().intFromStr(CONTENT_LENGTH);

		RequestBody *reqBody = RequestBodyFactory().build(rawRequestBuffer, rawRequestBuffer_Length, bodyLen, reqHeader);

		Request *request = RequestFactory().build(reqHeader, reqBody);
		request->setFdClient(e.getFdClient());
		e.setRequest(request);

	}
	return e;
}

