#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>

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
	//	TODO new
	processorLocator = new ProcessorLocator();
	ProcessorFactory processorFactory = ProcessorFactory(processorLocator);
	std::map<std::string, std::string> *locations = config->getParamStrStartingWith("location@");
	for (std::map<std::string, std::string>::iterator ite = locations->begin(); ite != locations->end(); ite++)
	{
		std::string key = ite->first;
		su.trim(key);
		std::string urlpath = su.getNthTokenIfExists(su.tokenize(key, '@'), 1, "");
		std::string val = ite->second;
		su.trim(val);

		std::vector<std::string> toksVal = su.tokenize(val, ';');
		for (std::vector<std::string>::iterator iteVal = toksVal.begin(); iteVal != toksVal.end(); iteVal++)
		{
			std::string directive = *iteVal;
			su.trim(directive);
			if (directive == "")
				continue;
			std::vector<std::string> toksDirective = su.tokenize(directive, ' ');
			std::string dirName = su.getNthTokenIfExists(toksDirective, 0, "");
			su.trim(dirName);
			Processor *processor;
			if (dirName == "SetHandler")
			{
				std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
				std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");
				processor = processorFactory.build(processorName);
				processor->setConfig(config);

				processorLocator->addLocationToProcessor(urlpath, extension, processor);
			}
			else
			{
				std::string nameProperty = su.getNthTokenIfExists(toksDirective, 0, "");
				std::string valProperty = su.getNthTokenIfExists(toksDirective, 1, "");
				processor->addProperty(nameProperty, valProperty);
			}
		}
	}
}

void HttpServer::onIncomming(ConnectorEvent e)
{
}

void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getTemp();
	RequestHeader *reqHeader = RequestHeaderFactory().build(&rawRequest);
	//seg fault
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

	int fdSocket = e.getFdClient();
	pushItIntoTheWire(fdSocket, request, resp);

	cleanUp(e, request, resp);

	//	if (bodyLen)
	//	{
	//		std::ofstream os("out2.html", std::ios::binary | std::ios::out);
	//		os.write(cstr, length);
	//		os.close();
	//	}
}

Response *HttpServer::runProcessorChain(std::vector<ProcessorAndLocationToProcessor *> *processorList, Request *request,
										Response *resp)
{
	for (std::vector<ProcessorAndLocationToProcessor *>::iterator ite = processorList->begin();
		 ite != processorList->end();
		 ite++)
	{
		ProcessorAndLocationToProcessor *processorAndLocationToProcessor = *ite;
		Processor *processor = processorAndLocationToProcessor->getProcessor(); //		processorList->at(0);
		harl.debug("HttpServer::runProcessorChain : injecting Config [%s]", config->getAlias().c_str());
		processor->setConfig(config);

		harl.debug("HttpServer::runProcessorChain : %s \t processing [%s]", request->getUri().c_str(),
				   processor->toString().c_str());
		resp = processor->process(request, resp, processorAndLocationToProcessor);
		//		delete processor;
	}
	return resp;
}

char *HttpServer::packageResponseAndGiveMeSomeBytes(Request *request, Response *resp)
{
	StringUtil stringUtil;
	std::string fieldsString = stringUtil.fromListToString(
		resp->getHeader()->getFields());
	std::string statusLine = resp->getHeader()->getStatusLine();

	// Send Response
	std::string statusHeaderBody = resp->getHeader()->getStatusLine() + fieldsString + std::string(resp->getBody());

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

// void HttpServer::onDataReceiving(ConnectorEvent e)
// {
// 	std::string rawRequest = e.getTemp();
// 	RequestHeader *reqHeader = RequestHeaderFactory().build(&rawRequest);
// 	Request *request = RequestFactory().build(reqHeader);
// 	request->setFdClient(e.getFdClient());
// 	//	req->dump();

// 	RequestHttp *httpReq = dynamic_cast<RequestHttp *>(request);
// 	if (httpReq != NULL)
// 	{
// 		std::string body = httpReq->getBody();
// 		std::string contentType = httpReq->getHeaderFieldValue("Content-Type");
// 		size_t boundaryPos = contentType.find("boundary=");

// 		if (boundaryPos != std::string::npos && contentType.find("multipart/form-data") != std::string::npos)
// 		{
// 			std::string boundary = contentType.substr(boundaryPos + 9);
// 			MultipartParser parser(boundary);
// 			std::string targetDir = "./uploads"; // Target directory for file uploads

// 			// Parse Multipart Data and save files
// 			parser.parseMultipartData(body, targetDir);

// 			// Generate and send response for successful file upload
// 			std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nFiles uploaded successfully.\r\n";
// 			send(e.getFdClient(), response.c_str(), response.length(), 0);

// 			delete request;
// 			return;
// 		}

void shutFd(int fd)
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}
