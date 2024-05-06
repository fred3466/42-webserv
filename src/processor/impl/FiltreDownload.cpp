#include "FiltreDownload.h"

FiltreDownload::FiltreDownload(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper()
{
	this->type = type;
}

FiltreDownload::~FiltreDownload()
{
	delete config;
}

void FiltreDownload::setConfig(Config *conf)
{
	config = conf;
}

ProcessorTypeEnum FiltreDownload::getType()
{
	return type;
}

std::string FiltreDownload::toString()
{
	return "FiltreDownload";
}

Response* FiltreDownload::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void) request;
	(void) processorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
//	std::string reqPath = request->getPath();
//	reqPath = StringUtil().trim(reqPath);
//
//	std::string target = config->getParamStr("target", "");
////	HttpError *error = HttpErrorFactory().build(errorCode);
////	response->setHttpError(error);
////	response->setStatusLine(error->getStatusLine());
//
//	header->setStatusLine("HTTP/1.1 307 Temporary Redirect\r\n");
	Uri uri = request->getUri();
	std::string filename = uri.getFileNameAndExt();
	header->addField("Content-Disposition", "attachment;filename=" + filename);

	return response;
}

void FiltreDownload::addProperty(std::string name, std::string value)
{
	config->addOrReplaceParam(name, value);
}

bool FiltreDownload::isExclusif()
{
	return false;
}

bool FiltreDownload::isBypassingExclusif()
{
	return false;
}

std::string FiltreDownload::getProperty(std::string name, std::string defaultVal)
{
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreDownload::getConfig()
{
	return config;
}
