#include "FiltreResponseRedirect.h"

FiltreResponseRedirect::FiltreResponseRedirect(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper() {
	this->type = type;
}

FiltreResponseRedirect::~FiltreResponseRedirect() {
	delete config;
}

void FiltreResponseRedirect::setConfig(Config *conf) {
	config = conf;
}

ProcessorTypeEnum FiltreResponseRedirect::getType() {
	return type;
}

std::string FiltreResponseRedirect::toString() {
	return "FiltreResponseRedirect";
}

Response* FiltreResponseRedirect::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) request;
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
	std::string reqPath = request->getPath();
	reqPath = StringUtil().trim(reqPath);

	std::string target = config->getParamStr("target", "");

	header->setStatusLine("HTTP/1.1 307 Temporary Redirect\r\n");
	header->addField("Location", target);
	response->setBodyBin(0);
	response->setErrorCodeTmp(307);
	response->setIsRedirect(true);

	return response;
}

bool FiltreResponseRedirect::isRedirect() {
	return true;
}

void FiltreResponseRedirect::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

bool FiltreResponseRedirect::isExclusif() {
	return true;
}

bool FiltreResponseRedirect::isBypassingExclusif() {
	return true;
}

bool FiltreResponseRedirect::isUriDirectoryValidationRequired() {
	return false;
}
bool FiltreResponseRedirect::isCgi() {
	return false;
}

std::string FiltreResponseRedirect::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreResponseRedirect::getConfig() {
	return config;
}
