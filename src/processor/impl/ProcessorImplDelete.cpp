#include "ProcessorImplDelete.h"

ProcessorImplDelete::ProcessorImplDelete(ProcessorTypeEnum type) : harl(), config() {
	this->type = type;
}

ProcessorImplDelete::~ProcessorImplDelete() {
	delete config;
}

Response* ProcessorImplDelete::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;

	Uri uri = request->getUri();
	harl.debug("Received DELETE request for URI: " + uri.getUri());
	std::string fileName = uri.getFileNameAndExt();
	std::string base_path = config->getParamStr("base_path", "/");
	std::string fullPath = FileUtil().realPathFile(base_path + "/" + fileName);

	harl.debug("ProcessorImplDelete::process: Full path resolved to: " + fullPath);

	std::ifstream file(fullPath.c_str());

	if (FileUtil().isDirectory(fullPath)) {
		harl.info("ProcessorImplDelete::process: File is a directory: " + fullPath);
		response->setErrorCodeTmp(400);
		return response;
	}

	if (!file.good()) {
		harl.info("ProcessorImplDelete::process: File not found at path: " + fullPath);
		response->setErrorCodeTmp(404);
		return response;
	}

	if (std::remove(fullPath.c_str()) != 0) {
		harl.error("ProcessorImplDelete::process: Failed to delete file at path: " + fullPath);
		response->setErrorCodeTmp(500);
	} else {
		harl.info("ProcessorImplDelete::process: Successfully deleted file at path: " + fullPath);
		response->setErrorCodeTmp(204);
	}
	return response;
}

bool ProcessorImplDelete::isCgi() {
	return false;
}
bool ProcessorImplDelete::isRedirect() {
	return false;
}

void ProcessorImplDelete::setConfig(Config *conf) {
	config = conf;
}

std::string ProcessorImplDelete::toString() {
	return "ProcessorImplDelete";
}

void ProcessorImplDelete::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplDelete::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

ProcessorTypeEnum ProcessorImplDelete::getType() {
	return type;
}

bool ProcessorImplDelete::isExclusif() {
	return true;
}

bool ProcessorImplDelete::isBypassingExclusif() {
	return false;
}

bool ProcessorImplDelete::isUriDirectoryValidationRequired() {
	return false;
}

Config* ProcessorImplDelete::getConfig() {
	return config;
}
