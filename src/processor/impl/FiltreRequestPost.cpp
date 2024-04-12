#include "FiltreRequestPost.h"

FiltreRequestPost::FiltreRequestPost(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper()
{
	this->type = type;
}

FiltreRequestPost::~FiltreRequestPost()
{
}

void FiltreRequestPost::setConfig(Config *conf)
{
	config = conf;
}

ProcessorTypeEnum FiltreRequestPost::getType()
{
	return type;
}

std::string FiltreRequestPost::toString()
{
	return "FiltreRequestPost " + type;
}
// TODO http error
/*
 If CONTENT_TYPE is blank, the script can reject the request
 with a 415 'Unsupported Media Type' error, where supported by the
 protocol.

 When parsing PATH_INFO, PATH_TRANSLATED or SCRIPT_NAME the script
 should be careful of void path segments ("//") and special path
 segments ("." and "..").  They should either be removed from the path
 before use in OS system calls, or the request should be rejected with
 404 'Not Found'.
 */
Response* FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	(void) processorAndLocationToProcessor;
	RequestBody *body = request->getBody();
	(void) body;
	//	response->set
	std::string CONTENT_TYPE = request->getHeaderFieldValue("Content-Type");
	if (CONTENT_TYPE == "application/x-www-form-urlencoded" || CONTENT_TYPE == "multipart/form-data")
	{
		//		POST POST POST POST POST POST POST POST POST POST POST

		std::string CONTENT_LENGTH = request->getHeaderFieldValue("Content-Length");
	}

	return response;
}

void FiltreRequestPost::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}

bool FiltreRequestPost::isExclusif()
{
	return false;
}

bool FiltreRequestPost::isBypassingExclusif()
{
	return false;
}
