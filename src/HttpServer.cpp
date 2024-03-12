#include "HttpServer.h"

#include <fstream>
#include <iostream>
#include <ostream>

HttpServer::HttpServer() : harl(), connector(), config()
{
}

HttpServer::~HttpServer()
{
}

//bool HttpServer::operator==(const ConnectorListener &o) {
////	if (o)
//	return this->_soListen ==  o._soListen;
////	return 0;
//}
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

	// config = ConfigFactory().build(fileConf);
	//	config.read("config.properties");
	config = c;

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

	Processor *processor = ProcessorFactory().build(request);
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

	//	std::strcpy(cstr, statusHeaderBody.c_str());

	//	std::memcpy(cstr, statusHeaderBody.c_str(), length);

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
