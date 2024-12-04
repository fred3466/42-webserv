#include "ProcessorImplCgiBinPerl.h"
#include "../../HttpServer.h"
#include "../CGI/CGIHelper.h"

ProcessorImplCgiBinPerl::ProcessorImplCgiBinPerl(ProcessorTypeEnum type) : harl(), stringUtil(), config(), fileUtil(), processorHelper() {
	this->type = type;
}
ProcessorImplCgiBinPerl::~ProcessorImplCgiBinPerl() {
	harl.debug("ProcessorImplCgiBinPerl::~ProcessorImplCgiBinPerl: destruction de  Config dans ProcessorImplCgiBinPerl");
	delete config;
}

void ProcessorImplCgiBinPerl::setConfig(Config *conf) {
	config = conf;
}

Response* ProcessorImplCgiBinPerl::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	response->getHeader()->addField("Content-Type", "text/html;");

	std::string base_path = config->getParamStr("base_path", "base_path_missing");
	std::string interpreterName = config->getParamStr("perl_exe", "");
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

	CGIHandler *cgiHandler = CGIHandlerFactory().build("PERL_CGI", config);
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

bool ProcessorImplCgiBinPerl::isRedirect() {
	return false;
}

std::string ProcessorImplCgiBinPerl::getBasePath() {
	return config->getParamStr("base_path", "cgi-bin");
}

void ProcessorImplCgiBinPerl::setBasePath(std::string basePath) {
	config->addParam("base_path", basePath);
}

void ProcessorImplCgiBinPerl::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

std::string ProcessorImplCgiBinPerl::toString() {
	return "ProcessorImplCgiBinPerl";
}

ProcessorTypeEnum ProcessorImplCgiBinPerl::getType() {
	return type;
}

bool ProcessorImplCgiBinPerl::isExclusif() {
	return true;
}

bool ProcessorImplCgiBinPerl::isCgi() {
	return true;
}

bool ProcessorImplCgiBinPerl::isBypassingExclusif() {
	return false;
}

bool ProcessorImplCgiBinPerl::isUriDirectoryValidationRequired() {
	return true;
}

std::string ProcessorImplCgiBinPerl::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* ProcessorImplCgiBinPerl::getConfig() {
	return config;
}
