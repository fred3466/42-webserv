#include "FiltreResponseMimeType.h"

#include <algorithm>

MimeTypeHelper FiltreResponseMimeType::mimeTypeHelper;

FiltreResponseMimeType::FiltreResponseMimeType(ProcessorTypeEnum type) : config(), type(type), processorHelper() {
}

FiltreResponseMimeType::~FiltreResponseMimeType() {
	delete config;
}

Response* FiltreResponseMimeType::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;
	ResponseHeader *header = response->getHeader();

	if (header) {
		std::string contentType = header->getFieldAsStr("Content-Type", "");
		if (contentType == "" || "application/octet-stream" == contentType) {
			std::string extension = request->getUri().getFileExtension();
			std::string mimeType = mimeTypeHelper.findMimeType(extension);
			header->addField("Content-Type", mimeType);
		}
	}

	return response;
}

bool FiltreResponseMimeType::isRedirect() {
	return false;
}

bool FiltreResponseMimeType::isCgi() {
	return false;
}

void FiltreResponseMimeType::setConfig(Config *conf) {
	this->config = conf;
}

std::string FiltreResponseMimeType::toString() {
	std::ostringstream oss;
	oss << "FiltreResponseMimeType";
	return oss.str(); // Convert the ostringstream to string and return it
}

void FiltreResponseMimeType::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

ProcessorTypeEnum FiltreResponseMimeType::getType() {
	return type;
}

bool FiltreResponseMimeType::isExclusif() {
	return false;
}

bool FiltreResponseMimeType::isBypassingExclusif() {
	return false;
}

bool FiltreResponseMimeType::isUriDirectoryValidationRequired() {
	return false;
}

std::string FiltreResponseMimeType::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreResponseMimeType::getConfig() {
	return config;
}
