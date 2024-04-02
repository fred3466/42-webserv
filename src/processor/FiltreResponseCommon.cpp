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
	path = request->getUri().getUri();

	//	PHP
	header->addField("Server", "webserv/Anastasia Jean-Baptiste Frederic");
	std::string date = stringUtil.formatDate(time(0), RFC_1123_DATE_FORMAT);
	header->addField("Date", date);
	//	Voir filtre MimeType
	//	header->addField("Content-Type", "");
	//	TODO fred post à vérifier 29/03
	//	TODO on s'en fout ??? => 02/04 fred: on dirait que non !
//	header->addField("Transfer-Encoding", "chunked"); // chunked
	//	le contraire de keep-alive
	//	TODO fred à vérifier 29/03
	bool isKeepAlive = request->isConnectionKeepAlive();
	header->addField("Connection", isKeepAlive ? "keep-alive" : "close");

	std::string directoryHandlingMethod = config->getParamStr("directory_default", "auto");
	if (fileUtil.isDirectory(path))
	{
		if (directoryHandlingMethod == "none")
		{
			HttpError *error = HttpErrorFactory::build(404);
			response->getHeader()->setStatusLine(error->getStatusLine());
			delete error;
			return response;
		}
		else if (directoryHandlingMethod == "auto")
		{
			std::string defaultFileName = config->getParamStr("default_index_name", "index.html");
			path += "/" + defaultFileName;
		}
		else
		{
			path += "/" + directoryHandlingMethod;
		}
	}

	//	html jpg
	//	TODO à implémenter ! fred => voir HttpServer
	//	int length = response->getBodyLength();
	//	std::string lString = stringUtil.strFromInt(length);
	//	header->addField("Content-Length", lString);
	//	TODO la date du fichier si statique , idem si html statique, absent si dynamique (php)
	header->addField("Last-Modified", "");
	//	TODO si implémenté, devrait être dans chaque implémentation de Processor (selon que c'est une ressource statique ou pas)
	header->addField("ETag", ""); // 65f5bc3b-17e9
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
