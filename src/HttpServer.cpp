#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>
#include <string>

#include "PollManager.h"

std::vector<ProcessorAndLocationToProcessor*> *HttpServer::processorList = NULL;
ProcessorLocator *HttpServer::processorLocator = new ProcessorLocator();
Harl HttpServer::harl;
StringUtil HttpServer::su;

HttpServer::HttpServer() : connector(), config(), requestHelper(NULL), bShareConnector(false) {
	errorFactory = HttpErrorFactory();
	name = "Server ???";
}

HttpServer::~HttpServer() {
	Harl().debug("HttpServer::~HttpServer: kill");
	if (!bShareConnector) {
		delete connector;
		connector = NULL;
	}
	delete processorLocator;
	processorLocator = NULL;
	delete config;
	config = NULL;
}

void delResponse(Response *resp) {
	Harl().warning("HttpServer::delResponse ");
	ServerRunner::delResp(resp);
}

void HttpServer::init(Config *c) {
	netStruct ns;

	config = c;
	instantiateProcessLocator();

	errorFactory = HttpErrorFactory();

	std::string server_name = c->getParamStr("server_name", "UNNAMED SERVER ?");
	std::string ip = c->getParamStr("listen", "127.0.0.1");
	std::string base_path = c->getParamStr("base_path", "");
	int port = c->getParamInt("port", 8080);

	name = "Server " + c->getAlias();

	harl.warning("+++ HttpServer::init: Starting server [%s] on [%s:%i] base_path : %s", server_name.c_str(), ip.c_str(), port, base_path.c_str());

	connector = ConnectorFactory().build(ip, port, c);

}

void HttpServer::instantiateProcessLocator() {
	StringUtil su = StringUtil();
	std::string port = config->getParamStr("port", "80");
	port = ":" + port;
	std::string host = config->getParamStr("server_name", "") + port;
	std::string hostAsIpPort = config->getParamStr("listen", "") + port;

	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::vector<std::string> *locations = config->getParamStrStartingWith("location@");
	for (std::vector<std::string>::iterator ite = locations->begin(); ite != locations->end(); ite++) {
		std::vector<std::string> keyValToks = su.tokenize(*ite, '|');
		std::string key = su.getNthTokenIfExists(keyValToks, 0, "");
		key = su.trim(key);
		std::string val = su.getNthTokenIfExists(keyValToks, 1, "");
		val = su.trim(val);

		std::string urlpath = su.getNthTokenIfExists(su.tokenize(key, '@'), 1, "");
		harl.info("HttpServer::instantiateProcessLocator:\t Processing %s", urlpath.c_str());

		std::vector<std::string> toksVal = su.tokenize(val, ';');
		std::vector<Processor*> processorsVector = std::vector<Processor*>();
		for (std::vector<std::string>::iterator iteVal = toksVal.begin(); iteVal != toksVal.end(); iteVal++) {
			std::string directive = *iteVal;
			directive = su.trim(directive);
			if (directive == "")
				continue;
			std::vector<std::string> toksDirective = su.tokenize(directive, ' ');
			std::string dirName = su.getNthTokenIfExists(toksDirective, 0, "");
			dirName = su.trim(dirName);
			//			Processor *processor;
			if (dirName == "SetHandler") {
				std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
				std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");

				Config *configProc = config->clone();
				std::string method = configProc->getParamStr("limit_except", "GET POST");

				Processor *processor = processorFactory.build(processorName, configProc);
				processorsVector.push_back(processor);

				processor->setConfig(configProc);

				harl.debug("HttpServer::instantiateProcessLocator:\t [%s \t %s \t %s \t %s]", urlpath.c_str(), extension.c_str(), processor->toString().c_str(), host.c_str());
				processorLocator->addLocationToProcessor(urlpath, extension, processor, host, hostAsIpPort, method);
			} else {
				std::string nameProperty = su.getNthTokenIfExists(toksDirective, 0, "");
				std::string valProperty = su.getNthTokenIfExists(toksDirective, 1, "");

				for (std::vector<Processor*>::iterator iteProcessor = processorsVector.begin(); iteProcessor != processorsVector.end(); iteProcessor++) {
					Processor *processor = *iteProcessor;
					processor->addProperty(dirName, valProperty);

					harl.trace("HttpServer::instantiateProcessLocator \t%s.addProperty(%s , %s)", processor->toString().c_str(), nameProperty.c_str(), valProperty.c_str());
				}
			}
		}
	} // for
	delete locations;

	harl.debug("");
	std::vector<LocationToProcessor*> locationToProcessorVector = processorLocator->getLocationToProcessorVector();
	for (std::vector<LocationToProcessor*>::iterator ite = locationToProcessorVector.begin(); ite != locationToProcessorVector.end(); ite++) {

		LocationToProcessor *lp = *ite;
		harl.info("ROUTE : %s", lp->toString().c_str());
	}
}

Response* HttpServer::createResponse(RequestResponseParam *requestResponseParam) {
	Request *request = requestResponseParam->getRequest();
	RequestHeader *reqHeader = request->getHeader();
	Uri uri = reqHeader->getUri();
	std::string host = reqHeader->getFieldValue("Host");
	std::string unknownHost = "<HOST ?>";

	CookieFactory().build(reqHeader);

	harl.debug("HttpServer::createResponse Request BODY: \ntaille=%i", request->getBody()->getLen());

	Response *resp = createEmptyResponse();
	int fd = requestResponseParam->getFdClient();
	resp->setFd(fd);

	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	processorList = processorFactory.build(request);

	if (processorList->size() == 0) {
		delResponse(resp);
		freeProcessorList();
		return NULL;
	}

	if (!request->isResponseAlreadySent()) {
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor = NULL;
		resp = runProcessorChain(request, resp, nextProcessorAndLocationToProcessor);
	}

//	freeProcessorList();
	return resp;
}

Response* HttpServer::createEmptyResponse() {
	Harl harl;
	StringUtil su;
	HttpErrorFactory errorFactory;
	ResponseHeader *header = ResponseHeaderFactory().build();
	Response *resp = ResponseFactory().build(header);

	HttpError *error = errorFactory.build(200);
	resp->setErrorCodeTmp(200);
	resp->setStatusLine(error->getStatusLine());

	return resp;
}

void HttpServer::freeProcessorList() {
	if (!processorList)
		return;

	harl.debug("HttpServer::freeProcessorList  : size=%i", processorList->size());
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator ite = processorList->begin(); ite != processorList->end(); ite++) {
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		delete (processorAndLocationToProcessor);
	}

	delete processorList;
	processorList = NULL;
}

bool contentDone = false;
bool oneIsExclusif = false;
bool oneIsMethodAllowed = false;
bool oneIsCGI = false;
Response* HttpServer::runProcessorChain(Request *request, Response *resp,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {

	if (request && request->isResponseAlreadySent())
		return resp;

	if (!nextProcessorAndLocationToProcessor) {
		contentDone = false;
		oneIsExclusif = false;
		oneIsMethodAllowed = false;
		oneIsCGI = false;

		for (std::vector<ProcessorAndLocationToProcessor*>::iterator ite = processorList->begin(); ite != processorList->end(); ite++) {
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
			Processor *processor = processorAndLocationToProcessor->getProcessor();
			bool bMethAllowed = processorLocator->_checkAccess(request, resp, processor);
			if (bMethAllowed) {
				oneIsMethodAllowed = true;
//				break;
			}
			if (processor->isCgi()) {
				oneIsCGI = true;
			}
		}

		if (!oneIsMethodAllowed) {
			harl.debug("HttpServer::runProcessorChain : No processor for method [%s]", request->getMethod().c_str());
			resp->setErrorCodeTmp(405);

			return resp;
		}
	}

	bool bIsCgi_SECOND_Pass = (nextProcessorAndLocationToProcessor) && resp->isBNeedFurtherProcessing();

	Uri uri = request->getUri();

	unsigned long indexProc = 0;
	if (nextProcessorAndLocationToProcessor) {
		Processor *proc = nextProcessorAndLocationToProcessor->getProcessor();
		std::string tos = proc->toString();
		harl.debug("HttpServer::runProcessorChain : retour de CGI vers proc=%s", tos.c_str());
		harl.debug("HttpServer::runProcessorChain : retour de CGI uri=%s bodylen=%i", uri.getUri().c_str(), resp->getBodyLength());

		for (indexProc = 0; indexProc < processorList->size(); indexProc++) {
			if (processorList->at(indexProc) == nextProcessorAndLocationToProcessor) {
//				if (indexProc < processorList->size() - 1) {
//					indexProc++;
//				}
//				resp->setBNeedFurtherProcessing(false);
				nextProcessorAndLocationToProcessor = NULL;
				break;
			}
		}
	}

	for (; indexProc < processorList->size(); indexProc++) {
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = processorList->at(indexProc);
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor = NULL;

		Processor *processor = processorAndLocationToProcessor->getProcessor();
		bool bBypassingExclusif = processor->isBypassingExclusif();
		bool metAllowed = processorLocator->_checkAccess(request, resp, processor);
		bool bProcIsExclusif = processor->isExclusif();
		bool bIsCgi_FIRST_pass = processor->isCgi() && (!resp->isBNeedFurtherProcessing());

		if (!metAllowed && bProcIsExclusif) {
			harl.debug("HttpServer::runProcessorChain : Exclusive processor [%s] refuse method [%s]", processor->toString().c_str(), request->getMethod().c_str());
			resp->setErrorCodeTmp(405);
			return resp;

		}

		if ((oneIsExclusif && !bBypassingExclusif) || (contentDone && (processor->getType() == CONTENT_MODIFIER)) || (!metAllowed)) {
			harl.debug("HttpServer::runProcessorChain : contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d metAllowed=%i ***[SKIPPING]*** [%s]"
					, contentDone, oneIsExclusif, bBypassingExclusif, metAllowed, processor->toString().c_str());
			continue;
		}

		harl.debug("HttpServer::runProcessorChain : %s \t processing [%s] contentDone=%d, oneIsExclusif=%d, bBypassingExclusif=%d", request->getUri().getUri().c_str(),
				processor->toString().c_str(),
				contentDone, oneIsExclusif, bBypassingExclusif);

		//Appel de chaque processor
		if (bIsCgi_FIRST_pass) {
			if (indexProc <= (processorList->size() - 1))
				nextProcessorAndLocationToProcessor = processorList->at(indexProc + 1);
			harl.debug("HttpServer::runProcessorChain : Mode CGI proc=%s", processor->toString().c_str(), request->getMethod().c_str());
			harl.debug("HttpServer::runProcessorChain : Mode CGI resp->setBNeedFurtherProcessing(true)");
			resp->setBNeedFurtherProcessing(true);
		}

		resp = processor->process(request, resp, processorAndLocationToProcessor, nextProcessorAndLocationToProcessor);

		resp->getHeader()->addField("X-webserv-proc" + su.strFromInt(indexProc), processor->toString());

		if (processor->isExclusif()) {
			harl.debug("HttpServer::runProcessorChain : %s est ***EXCLUSIF*** pour la constitution du corps de la réponse", processor->toString().c_str());
			oneIsExclusif = true;
		}

		if (!contentDone && processor->getType() == CONTENT_MODIFIER) {
			contentDone = true;
		}
//		CGI
		if (bIsCgi_FIRST_pass && (!bIsCgi_SECOND_Pass)) {
			harl.debug("HttpServer::runProcessorChain : Mode CGI arret de la boucle des proc");
			break;
		}
		if (bIsCgi_SECOND_Pass) {
			resp->setBNeedFurtherProcessing(false);
			CGIHelper::nextProcessorAndLocationToProcessor = NULL;
		}

	}				//fin for ################################

	if (!resp->isBNeedFurtherProcessing()) {
		freeProcessorList();

		std::string date = StringUtil().formatDate(time(0), RFC_1123_DATE_FORMAT);
		resp->getHeader()->addField("X-webserv-date", date);
		resp->getHeader()->addField("X-webserv-FD", StringUtil().strFromInt(request->getFdClient()));
		resp->getHeader()->addField("X-webserv-URI", uri.getUri());

		int errCode = resp->getErrorCodeTmp();
		if ((errCode != 200) && (!resp->isRedirect()) /*&& (!contentDone)*/) {
			harl.debug("HttpServer::runProcessorChain : HTTP error code different of 200 =%i", errCode);
			resp->setErrorCodeTmp(errCode);
			return resp;
		}
	}

	return resp;
}

void shutFd(int fd) {
	if (fd >= 0) {
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}

Connector* HttpServer::getConnector()
{
	return connector;
}

Config*& HttpServer::getConfig()
{
	return config;
}

void HttpServer::setConnector(Connector *connector) {
	if (this->connector != connector) {
		this->connector = connector;
		bShareConnector = true;
	}
}

std::string
HttpServer::getName()
{
	return name;
}

bool HttpServer::isBShareConnector() const
{
	return bShareConnector;
}

