#include "FiltreResponseCommon.h"

FiltreResponseCommon::FiltreResponseCommon(ProcessorTypeEnum type) : harl(), stringUtil(), config()
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
	return "FiltreResponseCommon";
}

Response* FiltreResponseCommon::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	ResponseHeader *header = response->getHeader();
	std::string path;
	path = request->getUri();

//	PHP
	header->addField("Server", "webserv/Anastasia Jean-Baptiste Frédéric");
//	TODO date de la requête, pour un jpg par exemple
	header->addField("Date", "");
//	Voir filtre MimeType
//	header->addField("Content-Type", "");
//	TODO on s'en fout
	header->addField("Transfer-Encoding", "");	//chunked
//	le contraire de keep-alive
	bool isKeepAlive = request->isConnectionKeepAlive();
	header->addField("Connection", isKeepAlive ? "keep-alive" : "close");

//	html jpg
//	TODO à implémenter ! fred
	header->addField("Content-Length", "");
//	TODO la date du fichier si statique , idem si html statique, absent si dynamique (php)
	header->addField("Last-Modified", "");
//	TODO si implémenté, devrait être dans chaque implémentation de Processor (selon que c'est une ressource statique ou pas)
	header->addField("ETag", "");	//65f5bc3b-17e9
//	TODO présent partout !
	header->addField("Accept-Ranges", "bytes");
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
