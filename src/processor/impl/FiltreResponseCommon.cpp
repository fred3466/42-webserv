#include "FiltreResponseCommon.h"

FiltreResponseCommon::FiltreResponseCommon(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper()
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

	header->addField("Server", "webserv/Anastasia Jean-Baptiste Frederic");
//	header->addField("Access-Control-Allow-Origin", "*");
	std::string date = stringUtil.formatDate(time(0), RFC_1123_DATE_FORMAT);
	header->addField("Date", date);
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

	int length = response->getBodyLength();
	std::string lString = stringUtil.strFromInt(length);
	header->addField("Content-Length", lString);
//	header->addField("Accept-Ranges", "bytes");

	return response;
}

void FiltreResponseCommon::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

bool FiltreResponseCommon::isExclusif()
{
	return false;
}

bool FiltreResponseCommon::isBypassingExclusif()
{
	return false;
}
