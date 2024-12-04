#include "FiltreDownload.h"

FiltreDownload::FiltreDownload(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper() {
	this->type = type;
}

FiltreDownload::~FiltreDownload() {
	delete config;
}

void FiltreDownload::setConfig(Config *conf) {
	config = conf;
}

ProcessorTypeEnum FiltreDownload::getType() {
	return type;
}

std::string FiltreDownload::toString() {
	return "FiltreDownload";
}

Response* FiltreDownload::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) request;
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();
	Uri uri = request->getUri();
	std::string filename = uri.getFileNameAndExt();
	header->addField("Content-Disposition", "attachment;filename=" + filename);

	return response;
}

bool FiltreDownload::isRedirect() {
	return false;
}

bool FiltreDownload::isCgi() {
	return false;
}

void FiltreDownload::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

bool FiltreDownload::isExclusif() {
	return false;
}

bool FiltreDownload::isBypassingExclusif() {
	return false;
}

bool FiltreDownload::isUriDirectoryValidationRequired() {
	return false;
}

std::string FiltreDownload::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreDownload::getConfig() {
	return config;
}
