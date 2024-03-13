#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <map>

HttpServer::HttpServer() : harl(), connector(), config(), processorFactory(NULL)
{
}

HttpServer::~HttpServer()
{
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
void HttpServer::init(Config *c)
{
	netStruct ns;
	StringUtil su = StringUtil();
//	TODO new
	ProcessorLocator *processorLocator = new ProcessorLocator();
	processorFactory = ProcessorFactory(processorLocator);

	// config = ConfigFactory().build(fileConf);
	//	config.read("config.properties");
	config = c;
	std::map<std::string, std::string> *locations = config->getParamStrStartingWith("location@");
	for (std::map<std::string, std::string>::iterator ite = locations->begin(); ite != locations->end(); ite++)
	{
		std::string key = ite->first;
		std::string urlpath = su.getNthTokenIfExists(su.tokenize(key, '@'), 1, "");
		std::string val = ite->second;
		std::vector<std::string> toksVal = su.tokenize(val, ';');
		for (std::vector<std::string>::iterator iteVal = toksVal.begin(); iteVal != toksVal.end(); iteVal++)
		{
			std::string directive = *iteVal;
			std::vector<std::string> toksDirective = su.tokenize(directive, ' ');
			std::string dirName = su.getNthTokenIfExists(toksDirective, 0, "");
			if (dirName == "SetHandler")
			{
				std::string processorName = su.getNthTokenIfExists(toksDirective, 1, "");
				std::string extension = su.getNthTokenIfExists(toksDirective, 2, "");
				Processor *processor = processorFactory.build(processorName);
				processor->setConfig(config);

				processorLocator->addLocationToProcessor(urlpath, extension, processor);
			}
		}

	}

	connector = ConnectorFactory().build(c->getParamStr("listen", "127.0.0.1"),
			c->getParamInt("port", 8080));
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::onIncomming(ConnectorEvent e)
{
}
void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getTemp();
	RequestHeader *reqHeader = RequestHeaderFactory().build(&rawRequest);
	Request *request = RequestFactory().build(reqHeader);
	request->setFdClient(e.getFdClient());
//	req->dump();

	std::vector<Processor*> *processorList = processorFactory.build(request);

	Processor *processor = processorList->at(0);
	processor->setConfig(config);
	Response *resp = processor->process(request);

	StringUtil stringUtil;
	std::string fieldsString = stringUtil.fromListToString(
			resp->getHeader()->getFields());
	std::string statusLine = resp->getHeader()->getStatusLine();

//Send Response
	std::string statusHeaderBody = resp->getHeader()->getStatusLine()
			+ fieldsString
			+ std::string(resp->getBody());

	int statusLen = statusLine.length();
	int headerLen = fieldsString.length();
	int bodyLen = resp->getBodyLength();

	int length = statusLen + headerLen + bodyLen;

	if (length <= 0)
	{
		delete request;
		delete processor;
		delete resp->getHeader();
		delete resp->getBodyBin();
		delete resp;
		return;
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

	//	if (bodyLen)
	//	{
	//		std::ofstream os("out2.html", std::ios::binary | std::ios::out);
	//		os.write(cstr, length);
	//		os.close();
	//	}

	int fd = e.getFdClient();
	if (request && fd && cstr && length)
		send(fd, cstr, length, 0);
	harl.debug("%d sent %d bytes through the wire", fd, length);
	harl.trace("%s", cstr);

	delete request;
	delete processor;
	delete resp->getHeader();
	delete resp->getBodyBin();
	delete resp;

}

//ProcessorLocator HttpServer::getProcessorLocator()
//{
//	return processorLocator;
//}
//
//void HttpServer::addLocationToProcessor(std::string ext, Processor *processor)
//{
//	processorLocator.addLocationToProcessor(ext, processor);
//}

