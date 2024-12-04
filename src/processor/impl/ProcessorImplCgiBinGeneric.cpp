#include "ProcessorImplCgiBinGeneric.h"
#include "../../HttpServer.h"
#include "../CGI/CGIHelper.h"

ProcessorImplCgiBinGeneric::ProcessorImplCgiBinGeneric(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper() {
	this->type = type;
}
ProcessorImplCgiBinGeneric::~ProcessorImplCgiBinGeneric() {
	harl.debug("ProcessorImplCgiBinGeneric::~ProcessorImplCgiBinGeneric: destruction de  Config dans ProcessorImplCgiBinGeneric");
	delete config;
}

void ProcessorImplCgiBinGeneric::setConfig(Config *conf) {
	config = conf;
}

Response* ProcessorImplCgiBinGeneric::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string interpreterName = config->getParamStr("generic_exe", "");
	std::string uriLessPathInfoAndQueryString = request->getUri().getPath() + request->getUri().getFileName() + request->getUri().getFileExtension();

	processorHelper.suppressRouteFromURI(processorAndLocationToProcessor, &uriLessPathInfoAndQueryString);
	std::string interpreterPath = interpreterName;
	std::string scriptPath;
	processorHelper.setScriptPath(&scriptPath, request, harl, (Processor*) this, base_path, uriLessPathInfoAndQueryString);

	if (!fileUtil.fileExists(interpreterPath)) {
		response->setErrorCodeTmp(500);
		return response;
	}

	CGIHandler *cgiHandler = CGIHandlerFactory().build("GENERIC_EXE", config);
	std::string cgiOutput = cgiHandler->executeCGIScript(interpreterPath, scriptPath, request, response);
	delete cgiHandler;

	if (nextProcessorAndLocationToProcessor) {
		CGIHelper::nextProcessorAndLocationToProcessor = nextProcessorAndLocationToProcessor;
	} else {
		bool bTransferEncoding = true; //"" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
		processorHelper.applyTransferEncodingOrContentLengthAndFinalize(response, &cgiOutput, bTransferEncoding);
	}
	return response;
}

bool ProcessorImplCgiBinGeneric::isRedirect() {
	return false;
}

std::string ProcessorImplCgiBinGeneric::getBasePath() {
	return config->getParamStr("base_path", "cgi-bin");
}

void ProcessorImplCgiBinGeneric::setBasePath(std::string basePath) {
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinGeneric::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplCgiBinGeneric::toString() {
	return "ProcessorImplCgiBinGeneric";
}

ProcessorTypeEnum ProcessorImplCgiBinGeneric::getType() {
	return type;
}

bool ProcessorImplCgiBinGeneric::isCgi() {
	return true;
}

bool ProcessorImplCgiBinGeneric::isExclusif() {
	return false;
}

bool ProcessorImplCgiBinGeneric::isBypassingExclusif() {
	return false;
}

bool ProcessorImplCgiBinGeneric::isUriDirectoryValidationRequired() {
	return true;
}

std::string ProcessorImplCgiBinGeneric::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* ProcessorImplCgiBinGeneric::getConfig() {
	return config;
}
