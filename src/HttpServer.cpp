#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>
#include <string>

HttpServer::HttpServer() : harl(), connector(), config(), processorLocator(), su(), requestHelper(NULL)
{
}

HttpServer::~HttpServer()
{
	for (std::vector<LocationToProcessor*>::iterator ite = processorLocator->getLocationToProcessorVector().begin();
			ite != processorLocator->getLocationToProcessorVector().end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		delete lp;
	}

	delete processorLocator;
	delete config;
}

void HttpServer::init(Config *c)
{
	netStruct ns;

	config = c;
	instantiateProcessLocator();

	errorFactory = HttpErrorFactory();

	std::string server_name = c->getParamStr("server_name", "UNAMED SERVER ?");
	std::string ip = c->getParamStr("listen", "127.0.0.1");
	std::string base_path = c->getParamStr("base_path", "");
	int port = c->getParamInt("port", 8080);

	harl.warning("+++ Starting server [%s] on [%s:%i] base_path : %s", server_name.c_str(), ip.c_str(), port, base_path.c_str());

	connector = ConnectorFactory().build(ip, port, c);
	connector->registerIt(this);

	connector->doListen();

//	delete config;
}

void HttpServer::instantiateProcessLocator()
{
	StringUtil su = StringUtil();
	std::string port = config->getParamStr("port", "80");
	port = ":" + port;
	std::string host = config->getParamStr("server_name", "") + port;

	//	TODO new
	processorLocator = new ProcessorLocator();
	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::vector<std::string> *locations = config->getParamStrStartingWith("location@");
	for (std::vector<std::string>::iterator ite = locations->begin(); ite != locations->end(); ite++)
	{
		std::vector<std::string> keyValToks = su.tokenize(*ite, '|');
		std::string key = su.getNthTokenIfExists(keyValToks, 0, "");
		key = su.trim(key);
		std::string val = su.getNthTokenIfExists(keyValToks, 1, "");
		val = su.trim(val);

		std::string urlpath = su.getNthTokenIfExists(su.tokenize(key, '@'), 1, "");
		harl.info("");
		harl.info("HttpServer::instantiateProcessLocator:\t Processing %s", urlpath.c_str());

		std::vector<std::string> toksVal = su.tokenize(val, ';');
		std::vector<Processor*> processorsVector = std::vector<Processor*>();
		for (std::vector<std::string>::iterator iteVal = toksVal.begin(); iteVal != toksVal.end(); iteVal++)
		{
			std::string directive = *iteVal;
			directive = su.trim(directive);
			if (directive == "")
				continue;
			std::vector<std::string> toksDirective = su.tokenize(directive, ' ');
			std::string dirName = su.getNthTokenIfExists(toksDirective, 0, "");
			dirName = su.trim(dirName);
			//			Processor *processor;
			if (dirName == "SetHandler")
			{
				std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
				std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");

				Config *configProc = config->clone();
				Processor *processor = processorFactory.build(processorName, configProc);
				std::string CUSTOM_CGI_EXE = configProc->getParamStr(processorName, "");
				processor->addProperty("CUSTOM_CGI_EXE", CUSTOM_CGI_EXE);
				processorsVector.push_back(processor);

//				processor->setConfig(configProc);

//				harl.debug("HttpServer::instantiateProcessLocator:\t [%s \t %s \t %s \t %s]", urlpath.c_str(), extension.c_str(), processor->toString().c_str(), host.c_str());
				processorLocator->addLocationToProcessor(urlpath, extension, processor, host);
			}
			else
			{
				std::string nameProperty = su.getNthTokenIfExists(toksDirective, 0, "");
				std::string valProperty = su.getNthTokenIfExists(toksDirective, 1, "");

				for (std::vector<Processor*>::iterator iteProcessor = processorsVector.begin(); iteProcessor != processorsVector.end(); iteProcessor++)
				{
					Processor *processor = *iteProcessor;
					processor->addProperty(dirName, valProperty);

					harl.debug("HttpServer::instantiateProcessLocator \t%s.addProperty(%s , %s)", processor->toString().c_str(), nameProperty.c_str(), valProperty.c_str());
				}
			}
		}
	} // for
	delete locations;

	harl.debug("");
	std::vector<LocationToProcessor*> locationToProcessorVector = processorLocator->getLocationToProcessorVector();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin();
			ite != locationToProcessorVector.end(); ite++)
	{

		LocationToProcessor *lp = *ite;
		harl.debug("ROUTE : %s", lp->toString().c_str());
	}
	//	Processor *defaultProcessor = processorFactory.build("STATIC_PROCESSOR");
	//	Config *configProc = config->clone();
	//	defaultProcessor->setConfig(configProc);
	//	processorLocator->addLocationToProcessor("/", ".", defaultProcessor, host);
}
// void addLocationToProcessor(
// std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
// std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");
// processor = processorFactory.build(processorName);
// processor->setConfig(config);
//
// processorLocator->addLocationToProcessor(urlpath, extension, processor, host);
//
// }
void HttpServer::onIncomming(ConnectorEvent *e)
{
	(void) e;
}
// TODO virifier Anastasia pour DELETE
bool HttpServer::_checkAccess(Request *request)
{
	std::string metReq = request->getHeader()->getMethod();
	std::string limitConfig = su.strUpperCase(config->getParamStr("limit_except", "POST GET DELETE"));
	std::vector<std::string> limit_exceptTab = su.tokenize(limitConfig, ' ');
	for (int i = 0; i < (int) limit_exceptTab.size(); i++)
	{
		std::string methConfig = limit_exceptTab[i];
		if (methConfig == metReq)
		{
			return true;
		}
	}
	harl.debug("HttpServer::_checkAccess: la méthode de la requête [%s] n'est pas autorisée : [%s]", metReq.c_str(), limitConfig.c_str());
	return false;
}

// bool HttpServer::checkRequestBodySize(Request *request, Response *&response)
// {
// 	std::string contentLengthStr = request->getHeader()->getFieldValue("Content-Length");
// 	int contentLength = StringUtil().intFromStr(contentLengthStr);
// 	int maxBodySize = this->config->getParamInt("max_body_size", 4096);

// 	if (contentLength > maxBodySize)
// 	{
// 		harl.error("Request for [%s] rejected: body size (%d bytes) exceeds the maximum allowed (%d bytes).",
// 				   request->getUri().getUri().c_str(), contentLength, maxBodySize);

// 		HttpError *error = HttpErrorFactory().build(413);
// 		if (!response)
// 		{
// 			ResponseHeader *header = ResponseHeaderFactory().build();
// 			response = ResponseFactory().build(header);
// 		}
// 		response->setHttpError(error);
// 		return false;
// 	}
// 	return true;
// }

// TODO Caca
bool HttpServer::checkRequestBodySize(Request *request, Response *&response)
{
	std::string contentLengthStr = request->getHeader()->getFieldValue("Content-Length");
	int contentLength = StringUtil().intFromStr(contentLengthStr);
	std::string uri = request->getUri().getUri();

	// Get the maximum body size specific to the route or use the default.
	//	int maxBodySize = this->config->getRouteSpecificMaxBodySize(uri, this->config->getParamInt("max_body_size", 4096));
	int maxBodySize = config->getParamInt("max_body_size", 4096);

	if (contentLength > maxBodySize)
	{
		harl.error("Request for [%s] rejected: body size (%d bytes) exceeds the maximum allowed (%d bytes).", uri.c_str(), contentLength, maxBodySize);
		HttpError *error = HttpErrorFactory().build(413);
		if (!response)
		{
			ResponseHeader *header = ResponseHeaderFactory().build();
			response = ResponseFactory().build(header);
		}
		response->setHttpError(error);
		return false;
	}
	return true;
}

static int ind = 0;

void HttpServer::onDataReceiving(ConnectorEvent *e)
{
	char *rawRequest = e->getByteBuffer();
	int rawRequestLen = e->getLen();
	Uri uri;
	int rawRequestBuffer_Length = -1;

	if (HARL_LEVEL >= 4)
	{
		FileUtil fu = FileUtil();
		std::string fname = "DBG/" + su.strFromInt(ind++) + "_request_frag.txt";
		fu.writeFile(fname.c_str(), rawRequest, rawRequestLen);
	}

	if (requestHelper == NULL)
	{
		requestHelper = new RequestHelper(rawRequest, rawRequestLen);
	}

	char *rawRequestBuffer = requestHelper->process(rawRequest, rawRequestLen);
	if (rawRequestBuffer == NULL)
	{
		delete[] rawRequestBuffer;
		return;
	} else
	{
		rawRequestBuffer_Length = requestHelper->getRawRequestBufferLength();

		if (HARL_LEVEL >= 4)
		{
			FileUtil fu = FileUtil();
			std::string fnameRequest = "DBG/onDataReceiving_defrag_request.txt";
			fu.writeFile(fnameRequest.c_str(), rawRequestBuffer, rawRequestBuffer_Length);
		}

//		delete requestHelper;
//		requestHelper = NULL;

	}

//	delete[] rawRequest;
//	rawRequest = NULL;

	RequestHeader *reqHeader = RequestHeaderFactory().build(rawRequestBuffer);
	uri = reqHeader->getUri();
	harl.debug("\nHttpServer::onDataReceiving :\n*******************\n%s\n*******************", (reqHeader->getMethod() + " " + uri.getUri()).c_str());
	harl.debug("\nHttpServer::onDataReceiving :HEADER HTTP:\n%s", reqHeader->toString().c_str());

	RequestBody *reqBody = RequestBodyFactory().build(rawRequestBuffer, rawRequestBuffer_Length, reqHeader);

//	delete[] rawRequestBuffer;

	Request *request = RequestFactory().build(reqHeader, reqBody);
	request->setFdClient(e->getFdClient());
	CookieFactory().build(reqHeader);
	int *fdSocket = e->getFdClient();

	harl.debug("HttpServer::onDataReceiving Request BODY: \n%s", request->getBody()->getContent());

//	TODO à virer
	Response *resp = createErrorResponse(200);

	if (!_checkAccess(request))
	{
		//		delete resp;
		resp = createErrorResponse(403);
		int *fdSocket = e->getFdClient();
		pushItIntoTheWire(fdSocket, request, resp);
		cleanUp(request, resp);
		return;
	}

	if (!checkRequestBodySize(request, resp))
	{
		//		delete resp;
		resp = createErrorResponse(413);
		pushItIntoTheWire(fdSocket, request, resp);
		cleanUp(request, resp);
		return;
	}

	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::vector<ProcessorAndLocationToProcessor*> *processorList = processorFactory.build(request);

	if (processorList->size() == 0)
	{
		harl.warning("HttpServer::onDataReceiving : No processor for host:port/route [%s]", uri.getUri().c_str());
		//		delete resp;
		resp = createErrorResponse(404);
		pushItIntoTheWire(fdSocket, request, resp);
		freeProcessorList(processorList);
		cleanUp(request, resp);
		return;
	}

	resp = runProcessorChain(processorList, request, resp);

	freeProcessorList(processorList);

	if (!resp)
	{
		harl.error("HttpServer::onDataReceiving : Problem with response for [%s]", uri.getUri().c_str());
		resp = createErrorResponse(500);
	}

	HttpError *httpError = resp->getHttpError();
	if (httpError)
	{
		int errCode = httpError->getCode();
		if (errCode != 200 && !resp->isRedirect())
		{
			//	http error traitée dans runProcessorChain
			harl.debug("HttpServer::onDataReceiving : Http Error response sent in runProcessorChain for code [%i]", errCode);
		}
	}

	int nbSent = pushItIntoTheWire(fdSocket, request, resp);
	if (!request->isConnectionKeepAlive() || (resp->isCgi() && nbSent == resp->getTotalLength()))
	{
		harl.debug("HttpServer::onDataReceiving : closeConnection fdSocket %i [%s]", *fdSocket, uri.getUri().c_str());
		connector->closeConnection(fdSocket);
	}
	cleanUp(request, resp);
}

void HttpServer::freeProcessorList(std::vector<ProcessorAndLocationToProcessor*> *processorList)
{
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator ite = processorList->begin();
			ite != processorList->end();
			ite++)
	{
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		delete (processorAndLocationToProcessor);
	}

	delete processorList;
}

Response* HttpServer::runProcessorChain(std::vector<ProcessorAndLocationToProcessor*> *processorList, Request *request, Response *resp)
{
	bool contentDone = false;
	bool oneIsExclusif = false;

	int indexProc = 0;
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator ite = processorList->begin();
			ite != processorList->end();
			ite++)

	{
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		Processor *processor = processorAndLocationToProcessor->getProcessor();
		bool bBypassingExclusif = processor->isBypassingExclusif();

		harl.debug("\n***");

		if ((oneIsExclusif && !bBypassingExclusif) || (contentDone && processor->getType() == CONTENT_MODIFIER))
		{
			harl.debug("HttpServer::runProcessorChain : contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d ***[SKIPPING]*** [%s]", contentDone, oneIsExclusif, bBypassingExclusif,
					processor->toString().c_str());
			continue;
		}

		harl.debug("HttpServer::runProcessorChain : %s \t processing [%s] contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d",
				request->getUri().getUri().c_str(),
				processor->toString().c_str(),
				contentDone,
				oneIsExclusif,
				bBypassingExclusif);

		Uri uri = request->getUri();
		if (/*method != "POST" && */uri.isDirectory())
		{
			harl.debug("HttpServer::runProcessorChain requested URI is a dir for [%s]", uri.getUri().c_str());
			std::string defaultIndexName = processor->getProperty("default_index_name", "NONE");
			harl.debug("HttpServer::runProcessorChain defaultIndexName=[%s]", defaultIndexName.c_str());
			if (defaultIndexName != "NONE")
			{
				uri.setFileNameAndExt(defaultIndexName);
				uri.updateUriStr();
				request->getHeader()->setUri(uri);
				harl.debug("HttpServer::runProcessorChain UPDATED uri=[%s]", uri.getUri().c_str());
			}
			else
			{
				harl.warning("HttpServer::onDataReceiving : defaultIndexName=NONE => 404", uri.getUri().c_str());
				//		delete resp;
				resp = createErrorResponse(404);
				//				pushItIntoTheWire(fdSocket, request, resp);
				//				cleanUp(request, resp);
				return resp;
			}
		}

		resp = processor->process(request, resp, processorAndLocationToProcessor);
		resp->getHeader()->addField("X-webserv-proc" + su.strFromInt(indexProc++) + ": ", processor->toString());

		int errCode = resp->getErrorCodeTmp();
		if (errCode != 200 && !resp->isRedirect())
		{
			harl.debug("HttpServer::runProcessorChain : HTTP error code different of 200, processing error page");

			delete resp;
			resp = createErrorResponse(errCode);
			return resp;
		}

		if (processor->isExclusif())
		{
			harl.debug("HttpServer::runProcessorChain : %s est ***EXCLUSIF*** pour la constitution du corps de la réponse", processor->toString().c_str());
			oneIsExclusif = true;
		}

		if (!contentDone && processor->getType() == CONTENT_MODIFIER)
		{
			contentDone = true;
		}
	}

	return resp;
}

Response* HttpServer::createErrorResponse(int errorCode)
{
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);

	HttpError *error = HttpErrorFactory().build(errorCode);
	resp->setHttpError(error);
	resp->setStatusLine(error->getStatusLine());
	resp->getHeader()->addField("Content-Type", "text/html;");

	if (errorCode != 200)
	{
		HttpReturnCodeHelper errHelper = HttpReturnCodeHelper(errorCode);
		char *pageContentArray;

		std::string errorPageFileNameKey = su.strFromInt(errorCode) + "_errorCodeFile";
		std::string errorPageFileName = config->getParamStr(errorPageFileNameKey, "");
		if (errorPageFileName == "")
		{
			errorPageFileName = config->getParamStr("_errorCodeFile", "");
		}

		int readRetCode = FileUtil().readFile(errorPageFileName, &pageContentArray);
		if (readRetCode == -1)
		{
//			TODO traiter l'erreur
			return resp;
		}
		std::string pageContent = std::string(pageContentArray);
		delete pageContentArray;
		errHelper.replacePlaceholders(pageContent, error->getDescription());

		char *bodybin = new char[pageContent.length() + 1];
		std::copy(pageContent.begin(), pageContent.end(), bodybin);
		bodybin[pageContent.length()] = '\0';
		resp->setBodyBin(bodybin);

		int len = pageContent.length();
		resp->setBodyLength(len);
	}

	return resp;
}

void HttpServer::addUltimateHeaders(Response *resp)
{
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

	if (transferEncoding != "" && contentLengthHeader != -1)
	{
		harl.error("HttpServer::addUltimateHeaders: présence des entêtes incompatibles\nTransfer-Encoding = [%s]\nContent-Length=[%i]", transferEncoding, contentLengthHeader);
	}
	if (contentLengthHeader != -1 && contentLengthHeader != contentLengthResponse)
	{
		harl.error(
				"HttpServer::addUltimateHeaders: Incohérence entre le Content-Length dans l'entête de la Response, et celui renvoyé par Response->getBodyLength():\ncontentLengthHeader=[%i]\ncontentLengthResponse=[%i]",
				contentLengthHeader, contentLengthResponse);
	}
//	TODO fred post 29/03
//	08/04 fred
	if (resp->isCgi())
		//		header->addField("Content-Length", su.strFromInt(contentLengthResponse));
		header->addField("Transfer-Encoding", "chunked");
	else // TODO fred : vraiment ?
	if (!resp->isRedirect() && transferEncoding == "" && contentLengthHeader == -1)
		header->addField("Content-Length", su.strFromInt(contentLengthResponse));
}

int HttpServer::pushItIntoTheWire(int *fdSocket, Request *request, Response *resp)
{
	char *cstr = packageResponseAndGiveMeSomeBytes(request, resp);
	int length = resp->getTotalLength();

	if (request && fdSocket && cstr && length)
	{
		send(*fdSocket, cstr, length, 0);
	}
	harl.debug("%d sent %d bytes through the wire", *fdSocket, length);
	harl.trace("%s", cstr);
	delete[] cstr;
	return length;
}

char* HttpServer::packageResponseAndGiveMeSomeBytes(Request *request, Response *resp)
{
	StringUtil stringUtil;

//		std::ostringstream oss;
//		oss << resp->getBodyLength();
//		std::string sizeStr = oss.str();
//
//		resp->getHeader()->addField("Content-Length", sizeStr);

	if (!resp || !resp->getHeader() /*|| resp->getHeader()->getStatusLine().empty()*/)
	{
		harl.error("HttpServer::packageResponseAndGiveMeSomeBytes : Problème avec response : \n[%s]", request->getUri().getUri().c_str());
	}

//	if (!resp->isRedirect())
	addUltimateHeaders(resp);
	std::list<std::string> *fields = resp->getHeader()->getFields();
	std::string fieldsString = stringUtil.fromListToString(fields, "") + "\r\n";
	delete fields;

	std::string statusLine = resp->getHeader()->getStatusLine();
	std::string body = "";
	char *bodyBin = resp->getBodyBin();
	if (bodyBin)
		body = std::string(bodyBin, resp->getBodyLength());

// Send Response
	std::string statusHeaderBody = resp->getHeader()->getStatusLine() + fieldsString + body;

	int statusLen = statusLine.length();
	int headerLen = fieldsString.length();
	int bodyLen = resp->getBodyLength();

	int length = statusLen + headerLen + bodyLen;
	resp->setTotalLength(length);

//	TODO fred: à mettre ailleurs
//	if (length <= 0)
//	{
//		delete request;
//		//		delete processor;
//		delete resp->getHeader();
//		delete resp->getBodyBin();
//		delete resp;
//		//		TODO gérer ce cas
//		harl.warning("Response de taille nulle ?");
//		return NULL;
//	}

	char *cstr = new char[length + 0]();
	char *cstrSave = cstr;
	char **cstrPtr = &cstr;

	std::memcpy(*cstrPtr, statusLine.c_str(), statusLen);
	*cstrPtr += statusLen + 0;
	std::memcpy(*(cstrPtr), fieldsString.c_str(), headerLen);
	*cstrPtr += headerLen + 0;
	std::memcpy(*(cstrPtr), resp->getBodyBin(), bodyLen);

	cstr = cstrSave;
	if (length)
	{
		std::ofstream os("out.html", std::ios::binary | std::ios::out);
		os.write(cstr, length);
		os.close();
	}
	return cstr;
}

void HttpServer::cleanUp(Request *request, Response *resp)
{
	delete requestHelper;
	requestHelper = NULL;

	delete processorLocator;
	delete request->getBody();
	delete request->getHeader();
	delete request;
	if (resp)
	{
		delete resp->getHttpError();
		delete resp->getHeader();
		//		TODO ne m'oubliez pas !
		delete[] resp->getBodyBin();
		delete resp;
	}
}

void shutFd(int fd)
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}

// Response *HttpServer::createErrorResponse(int errorCode)
// {
// 	ResponseHeader *header = ResponseHeaderFactory().build();
// 	Response *resp = ResponseFactory().build(header);

// 	HttpError *error = HttpErrorFactory().build(errorCode);
// 	resp->setHttpError(error);
// 	resp->setStatusLine(error->getStatusLine());
// 	return resp;
// }

Response* HttpServer::handleHttpError(int errorCode)
{
	HttpError *error = errorFactory.build(errorCode);
	Response *response = createErrorResponse(errorCode);
	response->setHttpError(error);
	response->setStatusLine(error->getStatusLine());
	return response;
}
