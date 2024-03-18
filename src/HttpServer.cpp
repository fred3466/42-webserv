#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>
#include <string>

HttpServer::HttpServer() : harl(), connector(), config(), processorLocator()
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

	connector = ConnectorFactory().build(c->getParamStr("listen", "127.0.0.1"),
										 c->getParamInt("port", 8080));
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::instantiateProcessLocator()
{
	StringUtil su = StringUtil();
	std::string port = config->getParamStr("port", "");
	if (port != "" && port != "80")
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

	}
	Processor *defaultProcessor = processorFactory.build("STATIC_PROCESSOR");
	Config *configProc = config->clone();
	defaultProcessor->setConfig(configProc);
	processorLocator->addLocationToProcessor("/", ".", defaultProcessor, host);
}
//void addLocationToProcessor(
//std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
//std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");
//processor = processorFactory.build(processorName);
//processor->setConfig(config);
//
//processorLocator->addLocationToProcessor(urlpath, extension, processor, host);
//
//}
void HttpServer::onIncomming(ConnectorEvent e)
{
}

void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getTemp();
	RequestHeader *reqHeader = RequestHeaderFactory().build(&rawRequest);
	// seg fault
	CookieFactory().build(reqHeader);
	Request *request = RequestFactory().build(reqHeader);
	request->setFdClient(e.getFdClient());
	//	req->dump();

	harl.info("HttpServer::onDataReceiving : %s", request->getUri().c_str());
	harl.debug("HttpServer::onDataReceiving : %s", request->getHeader()->toString().c_str());

	Response *resp;
	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::vector<ProcessorAndLocationToProcessor *> *processorList = processorFactory.build(request);
	resp = runProcessorChain(processorList, request, resp);

	if (!resp)
	{
		harl.error("HttpServer::onDataReceiving : Problème avec response : \n[%s]", request->getUri().c_str());
	}

	int fdSocket = e.getFdClient();
	pushItIntoTheWire(fdSocket, request, resp);

	cleanUp(e, request, resp);
}

Response *HttpServer::runProcessorChain(std::vector<ProcessorAndLocationToProcessor *> *processorList, Request *request,
										Response *resp)
{
	bool contentDone = false;
	for (std::vector<ProcessorAndLocationToProcessor*>::iterator ite = processorList->begin();
			ite != processorList->end();
			ite++)

	{
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		Processor *processor = processorAndLocationToProcessor->getProcessor(); //		processorList->at(0);
		if (contentDone && processor->getType() == CONTENT_MODIFIER)
		{
			harl.debug("HttpServer::runProcessorChain : contentDone==true, skipping [%s]", processor->toString().c_str());
			continue;
		}

//		harl.debug("HttpServer::runProcessorChain : injecting Config [%s]", config->getAlias().c_str());
//		processor->setConfig(config);

		harl.debug("HttpServer::runProcessorChain : %s \t processing [%s]", request->getUri().c_str(),
				   processor->toString().c_str());
		resp = processor->process(request, resp, processorAndLocationToProcessor);
		if (!contentDone && processor->getType() == CONTENT_MODIFIER)
		{
			contentDone = true;
		}
		//		delete processor;
	}
	return resp;
}

char *HttpServer::packageResponseAndGiveMeSomeBytes(Request *request, Response *resp)
{
	StringUtil stringUtil;

//	std::ostringstream oss;
//	oss << resp->getBodyLength();
//	std::string sizeStr = oss.str();
//
//	resp->getHeader()->addField("Content-Length", sizeStr);

	if (!resp || !resp->getHeader() || resp->getHeader()->getStatusLine().empty())
	{
		harl.error("HttpServer::packageResponseAndGiveMeSomeBytes : Problème avec response : \n[%s]", request->getUri().c_str());
	}

	std::string fieldsString = stringUtil.fromListToString(
								   resp->getHeader()->getFields()) +
							   "\r\n";
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

	if (length <= 0)
	{
		delete request;
		//		delete processor;
		delete resp->getHeader();
		delete resp->getBodyBin();
		delete resp;
		//		TODO gérer ce cas
		harl.warning("Response de taille nulle ?");
		return NULL;
	}

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

void HttpServer::pushItIntoTheWire(int fdSocket, Request *request, Response *resp)
{
	char *cstr = packageResponseAndGiveMeSomeBytes(request, resp);
	int length = resp->getTotalLength();

	if (request && fdSocket && cstr && length)
		send(fdSocket, cstr, length, 0);
	harl.debug("%d sent %d bytes through the wire", fdSocket, length);
	harl.trace("%s", cstr);
}

void HttpServer::cleanUp(ConnectorEvent e, Request *request, Response *resp)
{
	delete request;
	delete resp->getHeader();
	delete resp->getBodyBin();
	delete resp;
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
