#include "FiltreResponseCommon.h"

FiltreResponseCommon::FiltreResponseCommon(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper() {
	this->type = type;
}

FiltreResponseCommon::~FiltreResponseCommon() {
	delete config;
}

void FiltreResponseCommon::setConfig(Config *conf) {
	config = conf;
}

ProcessorTypeEnum FiltreResponseCommon::getType() {
	return type;
}

std::string FiltreResponseCommon::toString() {
	return "FiltreResponseCommon";
}

Response* FiltreResponseCommon::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
	std::string path;
	path = request->getUri().getUri();

	header->addNoReplaceField("Server", "webserv2024");
	std::string date = stringUtil.formatDate(time(0), RFC_1123_DATE_FORMAT);
	header->addNoReplaceField("Date", date);
	bool isKeepAlive = request->isConnectionKeepAlive();
	header->addNoReplaceField("Connection", isKeepAlive ? "keep-alive" : "close");

	int length = response->getBodyLength();
	std::string lString = stringUtil.strFromInt(length);
	header->addNoReplaceField("Content-Length", lString);
	header->addField("Accept-Ranges", "bytes");

	return response;
}

bool FiltreResponseCommon::isCgi() {
	return false;
}

bool FiltreResponseCommon::isRedirect() {
	return false;
}

void FiltreResponseCommon::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

bool FiltreResponseCommon::isExclusif() {
	return false;
}

bool FiltreResponseCommon::isBypassingExclusif() {
	return false;
}

bool FiltreResponseCommon::isUriDirectoryValidationRequired() {
	return false;
}

std::string FiltreResponseCommon::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreResponseCommon::getConfig() {
	return config;
}
