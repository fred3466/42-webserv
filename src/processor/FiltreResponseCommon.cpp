#include "FiltreResponseCommon.h"

FiltreResponseCommon::FiltreResponseCommon(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil()
{
	this->type = type;
}

FiltreResponseCommon::~FiltreResponseCommon()
{
}

void FiltreResponseCommon::setConfig(Config *conf)
{
	config = conf;
}

ProcessorTypeEnum FiltreResponseCommon::getType()
{
	return type;
}

std::string FiltreResponseCommon::toString()
{
	return "FiltreResponseCommon " + type;
}

Response* FiltreResponseCommon::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void) processorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
	std::string path;
	path = request->getUri();

//	PHP
	header->addField("Server", "webserv/Anastasia Jean-Baptiste Fr%E9d%E9ric");
	std::string date = stringUtil.formatDate(time(0), RFC_1123_DATE_FORMAT);
	header->addField("Date", date);
//	Voir filtre MimeType
//	header->addField("Content-Type", "");
	//	TODO fred post à vérifier 29/03
	//	TODO on s'en fout ???
	header->addField("Transfer-Encoding", "chunked");	//chunked
//	le contraire de keep-alive
//	TODO fred à vérifier 29/03
	bool isKeepAlive = request->isConnectionKeepAlive();
	header->addField("Connection", isKeepAlive ? "keep-alive" : "close");

//	html jpg
//	TODO à implémenter ! fred => voir HttpServer
//	int length = response->getBodyLength();
//	std::string lString = stringUtil.strFromInt(length);
//	header->addField("Content-Length", lString);
//	TODO la date du fichier si statique , idem si html statique, absent si dynamique (php)
	//	TODO fred à vérifier 29/03
//header->addField("Last-Modified", "");
//	TODO si implémenté, devrait être dans chaque implémentation de Processor (selon que c'est une ressource statique ou pas)
	//	TODO fred à vérifier 29/03
//	header->addField("ETag", "");	//65f5bc3b-17e9
//	TODO présent partout !
//	TODO vraiment à confronter avec nginx
//	header->addField("Accept-Ranges", "bytes");
//	header->addField("", "");
//	header->addField("", "");
//	header->addField("", "");
//	header->addField("", "");
//	header->addField("", "");
//	header->addField("", "");
//	header->addField("", "");

	return response;
}

void FiltreResponseCommon::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}
