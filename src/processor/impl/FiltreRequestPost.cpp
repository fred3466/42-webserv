#include "FiltreRequestPost.h"

FiltreRequestPost::FiltreRequestPost(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper() {
	this->type = type;
}

FiltreRequestPost::~FiltreRequestPost() {
	delete config;
}

void FiltreRequestPost::setConfig(Config *conf) {
	config = conf;
}

ProcessorTypeEnum FiltreRequestPost::getType() {
	return type;
}

std::string FiltreRequestPost::toString() {
	return "FiltreRequestPost";
}
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
Response* FiltreRequestPost::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;
	RequestBody *body = request->getBody();
	std::string CONTENT_TYPE = request->getHeaderFieldValue("Content-Type");
	std::string multiStr = "multipart/form-data";
	int pos = 0;
	int len = multiStr.length();
	int multiStrMatch = (0 == CONTENT_TYPE.compare(pos, len, multiStr));

	if (CONTENT_TYPE == "application/x-www-form-urlencoded" || multiStrMatch) {
		std::string basePath = config->getParamStr("base_path", "base_path");
		basePath = fileUtil.realPathFile(basePath);
		std::vector<std::string> vecTemp = stringUtil.tokenize(CONTENT_TYPE, ';');
		std::string boundary = vecTemp.at(1);
		boundary = stringUtil.replace_all(boundary, " boundary=", "");
		MultipartParser parser = MultipartParser(boundary);
		parser.parseMultipartData(body->getContent(), body->getLen(), basePath);

		std::string CONTENT_LENGTH = request->getHeaderFieldValue("Content-Length");
	} else {
		harl.debug("FiltreRequestPost::process: CONTENT_TYPE=%s", CONTENT_TYPE.c_str());
	}

	return response;
}

bool FiltreRequestPost::isRedirect() {
	return false;
}

void FiltreRequestPost::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

bool FiltreRequestPost::isCgi() {
	return false;
}

bool FiltreRequestPost::isExclusif() {
	return false;
}

bool FiltreRequestPost::isBypassingExclusif() {
	return false;
}

bool FiltreRequestPost::isUriDirectoryValidationRequired() {
	return false;
}

std::string FiltreRequestPost::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreRequestPost::getConfig() {
	return config;
}
