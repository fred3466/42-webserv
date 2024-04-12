#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>
#include <string>

HttpServer::HttpServer() : harl(), connector(), config(), processorLocator(), su()
{
}

HttpServer::~HttpServer()
{
}

void HttpServer::init(Config *c)
{
	netStruct ns;

	config = c;
	instantiateProcessLocator();

	HttpErrorFactory().initialize();

	connector = ConnectorFactory().build(c->getParamStr("listen", "127.0.0.1"), c->getParamInt("port", 8080), c);
	connector->registerIt(this);

	connector->doListen();

	delete config;
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
	std::map<std::string, std::string> *locations = config->getParamStrStartingWith("location@");
	for (std::map<std::string, std::string>::iterator ite = locations->begin(); ite != locations->end(); ite++)
	{
		std::string key = ite->first;
		key = su.trim(key);
		std::string urlpath = su.getNthTokenIfExists(su.tokenize(key, '@'), 1, "");
		std::string val = ite->second;
		val = su.trim(val);

		std::vector<std::string> toksVal = su.tokenize(val, ';');
		for (std::vector<std::string>::iterator iteVal = toksVal.begin(); iteVal != toksVal.end(); iteVal++)
		{
			std::string directive = *iteVal;
			directive = su.trim(directive);
			if (directive == "")
				continue;
			std::vector<std::string> toksDirective = su.tokenize(directive, ' ');
			std::string dirName = su.getNthTokenIfExists(toksDirective, 0, "");
			dirName = su.trim(dirName);
			Processor *processor;
			if (dirName == "SetHandler")
			{
				std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
				std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");
				processor = processorFactory.build(processorName);
				Config *configProc = config->clone();
				processor->setConfig(configProc);

				processorLocator->addLocationToProcessor(urlpath, extension, processor, host);
			}
			else
			{
				std::string nameProperty = su.getNthTokenIfExists(toksDirective, 0, "");
				std::string valProperty = su.getNthTokenIfExists(toksDirective, 1, "");
				processor->addProperty(nameProperty, valProperty);
			}
		}
	} // for
	delete locations;

	std::vector<LocationToProcessor *> locationToProcessorVector = processorLocator->getLocationToProcessorVector();
	for (std::vector<LocationToProcessor *>::iterator ite = locationToProcessorVector.begin();
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
void HttpServer::onIncomming(ConnectorEvent e)
{
	(void)e;
}

bool HttpServer::_checkAccess(Request *request)
{
	std::string metReq = request->getHeader()->getMethod();
	std::string limitConfig = su.strUpperCase(config->getParamStr("limit_except", "POST GET"));
	std::vector<std::string> limit_exceptTab = su.tokenize(limitConfig, ' ');
	for (int i = 0; i < (int)limit_exceptTab.size(); i++)
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

bool HttpServer::checkRequestBodySize(Request *request, Response *&response)
{
	std::string contentLengthStr = request->getHeader()->getFieldValue("Content-Length");
	int contentLength = StringUtil().intFromStr(contentLengthStr);
	int maxBodySize = this->config->getParamInt("max_body_size", 4096);

	if (contentLength > maxBodySize)
	{
		harl.error("Request for [%s] rejected: body size (%d bytes) exceeds the maximum allowed (%d bytes).",
				   request->getUri().getUri().c_str(), contentLength, maxBodySize);

		if (!response)
		{
			response = createErrorResponse(413);
		}
		else
		{
			HttpError *error = HttpErrorFactory().build(413);
			response->setHttpError(error);
			response->setStatusLine(error->getStatusLine());
		}
		return false;
	}
	return true;
}

// bool HttpServer::checkRequestBodySize(Request *request, Response *&response)
// {
// 	std::string contentLengthStr = request->getHeader()->getFieldValue("Content-Length");
// 	int contentLength = StringUtil().intFromStr(contentLengthStr);

// 	// Dynamically determine the server block key based on the request
// 	std::string serverBlockKey = determineServerBlockKey(request);
// 	std::string configKey = serverBlockKey + ".max_body_size";

// 	// Fetch the max body size from config without default here
// 	int maxBodySize;
// 	try
// 	{
// 		maxBodySize = this->config->getParamInt(configKey);
// 		if (maxBodySize <= 0)
// 		{
// 			throw std::runtime_error("Invalid max body size configuration.");
// 		}
// 	}
// 	catch (const std::exception &e)
// 	{
// 		harl.error("Configuration error for [%s]: %s", serverBlockKey.c_str(), e.what());
// 		if (!response)
// 		{
// 			response = createErrorResponse(500); // 500 Internal Server Error
// 		}
// 		else
// 		{
// 			HttpError *error = HttpErrorFactory().build(500);
// 			response->setHttpError(error);
// 			response->setStatusLine(error->getStatusLine());
// 		}
// 		return false;
// 	}

// 	if (contentLength > maxBodySize)
// 	{
// 		harl.error("Request for [%s] rejected: body size (%d bytes) exceeds the maximum allowed (%d bytes).",
// 				   request->getUri().getUri().c_str(), contentLength, maxBodySize);

// 		if (!response)
// 		{
// 			response = createErrorResponse(413);
// 		}
// 		else
// 		{
// 			HttpError *error = HttpErrorFactory().build(413);
// 			response->setHttpError(error);
// 			response->setStatusLine(error->getStatusLine());
// 		}
// 		return false;
// 	}
// 	return true;
// }

void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getByteBuffer();
	RequestHeader *reqHeader = RequestHeaderFactory().build(&rawRequest);
	RequestBody *reqBody = RequestBodyFactory().build(&rawRequest, reqHeader);
	Request *request = RequestFactory().build(reqHeader, reqBody);
	request->setFdClient(e.getFdClient());
	// seg fault
	CookieFactory().build(reqHeader);
	//	req->dump();

	harl.debug("\nHttpServer::onDataReceiving :\n*******************\n%s\n*******************", request->getUri().getUri().c_str());
	//	harl.debug("HttpServer::onDataReceiving Request HEADER: \n%s", request->getHeader()->toString().c_str());
	harl.debug("HttpServer::onDataReceiving Request BODY: \n%s", request->getBody()->getContent()->c_str());

	if (!_checkAccess(request))
	{
		Response *resp = createErrorResponse(403);
		int *fdSocket = e.getFdClient();
		pushItIntoTheWire(fdSocket, request, resp);
		cleanUp(request, resp);
		return;
	}

	Response *resp = createErrorResponse(200);

	if (!checkRequestBodySize(request, resp))
	{
		int *fdSocket = e.getFdClient();
		pushItIntoTheWire(fdSocket, request, resp);
		cleanUp(request, resp);
		return;
	}

	int errorCode = 200;
	HttpError *error = HttpErrorFactory().build(errorCode);
	resp->setHttpError(error);

	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::vector<ProcessorAndLocationToProcessor *> *processorList = processorFactory.build(request);

	resp = runProcessorChain(processorList, request, resp);
	delete processorList;

	if (!resp)
	{
		harl.error("HttpServer::onDataReceiving : Problème avec response : \n[%s]", request->getUri().getUri().c_str());
	}

	//	TODO Anastasia: ça devrait se faire seulement dans le filtre, et non pas en plusieurs endroits comme ici...
	HttpError *he = resp->getHttpError();
	resp->setStatusLine(he->getStatusLine());

	int *fdSocket = e.getFdClient();
	int nbSent = pushItIntoTheWire(fdSocket, request, resp);

	//	TODO Fred keepalive
	if (!request->isConnectionKeepAlive() || (resp->isCgi() && nbSent == resp->getTotalLength()))
	{
		harl.debug("HttpServer::onDataReceiving : closeConnection fdSocket %i [%s]", *fdSocket, request->getUri().getUri().c_str());
		connector->closeConnection(fdSocket);
	}

	//	cleanUp(request, resp);
}

Response *HttpServer::runProcessorChain(std::vector<ProcessorAndLocationToProcessor *> *processorList, Request *request,
										Response *resp)
{
	bool contentDone = false;
	bool oneIsExclusif = false;
	for (std::vector<ProcessorAndLocationToProcessor *>::iterator ite = processorList->begin();
		 ite != processorList->end();
		 ite++)

	{
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		Processor *processor = processorAndLocationToProcessor->getProcessor(); //		processorList->at(0);
		bool bBypassingExclusif = processor->isBypassingExclusif();

		if ((oneIsExclusif && !bBypassingExclusif) || (contentDone && processor->getType() == CONTENT_MODIFIER))
		{
			harl.debug("HttpServer::runProcessorChain : contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d skipping [%s]", contentDone, oneIsExclusif, bBypassingExclusif,
					   processor->toString().c_str());
			continue;
		}

		//		harl.debug("HttpServer::runProcessorChain : injecting Config [%s]", config->getAlias().c_str());
		//		processor->setConfig(config);

		harl.debug("HttpServer::runProcessorChain : %s \t processing [%s] contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d",
				   request->getUri().getUri().c_str(),
				   processor->toString().c_str(),
				   contentDone,
				   oneIsExclusif,
				   bBypassingExclusif);

		resp = processor->process(request, resp, processorAndLocationToProcessor);
		if (processor->isExclusif())
		{
			harl.debug("HttpServer::runProcessorChain : %s est EXCLUSIF", processor->toString().c_str());
			oneIsExclusif = true;
		}

		if (!contentDone && processor->getType() == CONTENT_MODIFIER)
		{
			contentDone = true;
		}
	}
	return resp;
}

void HttpServer::addUltimateHeaders(Response *resp)
{
	ResponseHeader *header = resp->getHeader();

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
		if (transferEncoding == "" && contentLengthHeader == -1)
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

char *HttpServer::packageResponseAndGiveMeSomeBytes(Request *request, Response *resp)
{
	StringUtil stringUtil;

	//		std::ostringstream oss;
	//		oss << resp->getBodyLength();
	//		std::string sizeStr = oss.str();
	//
	//		resp->getHeader()->addField("Content-Length", sizeStr);

	if (!resp || !resp->getHeader() || resp->getHeader()->getStatusLine().empty())
	{
		harl.error("HttpServer::packageResponseAndGiveMeSomeBytes : Problème avec response : \n[%s]", request->getUri().getUri().c_str());
	}

	addUltimateHeaders(resp);
	std::list<std::string> *fields = resp->getHeader()->getFields();
	std::string fieldsString = stringUtil.fromListToString(fields) + "\r\n";
	delete fields;

	std::string statusLine = resp->getHeader()->getStatusLine();
	std::string body = "";
	char *bodyBin = resp->getBodyBin();
	if (bodyBin)
		body = std::string(bodyBin);

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
	//	if (length)
	//	{
	//		std::ofstream os("out.html", std::ios::binary | std::ios::out);
	//		os.write(cstr, length);
	//		os.close();
	//	}
	return cstr;
}

void HttpServer::cleanUp(Request *request, Response *resp)
{
	delete request->getBody();
	delete request->getHeader();
	delete request;
	if (resp)
	{
		delete resp->getHttpError();
		delete resp->getHeader();
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

Response *HttpServer::createErrorResponse(int errorCode)
{
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);

	HttpError *error = HttpErrorFactory().build(errorCode);
	resp->setHttpError(error);
	resp->setStatusLine(error->getStatusLine());
	return resp;
}

std::string HttpServer::determineServerBlockKey(Request *request)
{
	std::string hostname = request->getHeader()->getFieldValue("Host");
	if (hostname.empty() || hostname == "default")
	{
		return "default"; // Ensure there is a 'default' configuration in your system
	}
	if (hostname == "s1.org")
	{
		return "s1";
	}
	if (hostname == "s2.org")
	{
		return "s1"; // Assuming you want to map s2.org to s1 configuration for now
	}
	return "default"; // Fallback to default if no matching hostname
}

// std::string HttpServer::determineServerBlockKey(Request *request)
// {
// 	// Get the hostname from the HTTP request
// 	std::string hostname = request->getHeader()->getFieldValue("Host");

// 	// Example logic to determine the server block key based on the hostname
// 	if (hostname.empty())
// 	{
// 		return "default"; // Use a default if no hostname is provided
// 	}

// 	// Simple mapping based on hostname
// 	if (hostname == "s1.org")
// 	{
// 		return "s1";
// 	}
// 	else if (hostname == "s2.org")
// 	{
// 		return "s2";
// 	}

// 	// Add more conditions as necessary for different hostnames

// 	// Default fallback
// 	return "default";
// }
