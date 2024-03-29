#include "FiltreRequestPost.h"

FiltreRequestPost::FiltreRequestPost()
{
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
	return "FiltreRequestPost";
}

Response* FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
	std::string rawBody = "";

	RequestBody *body = RequestBodyFactory().build(rawBody);
	request->setBody(body);

	return response;
}

void FiltreRequestPost::addProperty(std::string name, std::string value)
{
	config->addParam(name, value);
}
