#include "ProcessorImplCgiBinSh.h"
#include "../../HttpServer.h"
#include "../CGI/CGIHelper.h"

ProcessorImplCgiBinSh::ProcessorImplCgiBinSh(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper() {
	this->type = type;
}
ProcessorImplCgiBinSh::~ProcessorImplCgiBinSh() {
	delete config;
}

void ProcessorImplCgiBinSh::setConfig(Config *conf) {
	config = conf;
}

Response* ProcessorImplCgiBinSh::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	response->getHeader()->addField("Content-Type", "text/html;");

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string interpreterName = config->getParamStr("sh_exe", "");
	std::string uriLessPathInfoAndQueryString = request->getUri().getPath() + request->getUri().getFileName() + request->getUri().getFileExtension();

	processorHelper.suppressRouteFromURI(processorAndLocationToProcessor, &uriLessPathInfoAndQueryString);
	std::string interpreterPath = interpreterName;
	std::string scriptPath;
	processorHelper.setScriptPath(&scriptPath, request, harl, (Processor*) this, base_path, uriLessPathInfoAndQueryString);

	if (!fileUtil.fileExists(interpreterPath)) {
		response->setErrorCodeTmp(500);
		return response;
	}
	std::string scriptPathAbsolute = FileUtil().realPathFile(base_path + "/" + uriLessPathInfoAndQueryString);
	if (!fileUtil.fileExists(scriptPathAbsolute)) {
		response->setErrorCodeTmp(404);
		return response;
	}

	CGIHandler *cgiHandler = CGIHandlerFactory().build("SH_CGI", config);
	std::string cgiOutput = cgiHandler->executeCGIScript(interpreterPath, scriptPath, request, response);

	delete cgiHandler;

	if (nextProcessorAndLocationToProcessor) {
		CGIHelper::nextProcessorAndLocationToProcessor = nextProcessorAndLocationToProcessor;
	} else {
		bool bTransferEncoding = true; //"" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
//	bool bTransferEncoding = true; // "" != response->getHeader()->getFieldAsStr("Transfer-Encoding", "");
		processorHelper.applyTransferEncodingOrContentLengthAndFinalize(response, &cgiOutput, bTransferEncoding);
	}

	return response;
}

bool ProcessorImplCgiBinSh::isRedirect() {
	return false;
}

std::string ProcessorImplCgiBinSh::getBasePath() {
	return config->getParamStr("base_path", "cgi-bin/toto/");
}

void ProcessorImplCgiBinSh::setBasePath(std::string basePath) {
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinSh::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplCgiBinSh::toString() {
	return "ProcessorImplCgiBinSh";
}

ProcessorTypeEnum ProcessorImplCgiBinSh::getType() {
	return type;
}

bool ProcessorImplCgiBinSh::isExclusif() {
	return true;
}

bool ProcessorImplCgiBinSh::isCgi() {
	return true;
}

bool ProcessorImplCgiBinSh::isBypassingExclusif() {
	return false;
}

bool ProcessorImplCgiBinSh::isUriDirectoryValidationRequired() {
	return true;
}

std::string ProcessorImplCgiBinSh::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* ProcessorImplCgiBinSh::getConfig() {
	return config;
}
