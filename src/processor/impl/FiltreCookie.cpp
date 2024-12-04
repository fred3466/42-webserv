#include "FiltreCookie.h"

FiltreCookie::FiltreCookie(ProcessorTypeEnum type) : harl(), config(), fileUtil(), stringUtil(), processorHelper() {
	this->type = type;
}

FiltreCookie::~FiltreCookie() {
	delete config;
}

void FiltreCookie::setConfig(Config *conf) {
	config = conf;
}

ProcessorTypeEnum FiltreCookie::getType() {
	return type;
}

std::string FiltreCookie::toString() {
	return "FiltreCookie";
}

Response* FiltreCookie::process(Request *request, Response *response, ProcessorAndLocationToProcessor *processorAndLocationToProcessor,
		ProcessorAndLocationToProcessor *nextProcessorAndLocationToProcessor) {

	(void) request;
	(void) processorAndLocationToProcessor;
	(void) nextProcessorAndLocationToProcessor;

	std::list<Cookie> cookies = response->getHeader()->getCookiesList();
	for (std::list<Cookie>::const_iterator it = cookies.begin(); it != cookies.end(); it++) {
		const Cookie cook = *it;

		std::string cookieName = cook.getName();
		if (cookieName == "")
			continue;

		std::string cookyStr = cook.getCookie();
		harl.info("cookieString: %s", cookyStr.c_str());
		ResponseHeader *respHeader = response->getHeader();
		respHeader->addField("Set-Cookie", cookyStr);
		harl.debug("FiltreCookie::process: Ajout du cookie dans la resp : %s", cookyStr.c_str());
	}

	return response;
}

bool FiltreCookie::isCgi() {
	return false;
}

bool FiltreCookie::isRedirect() {
	return false;
}

void FiltreCookie::addProperty(std::string name, std::string value) {
	config->addOrReplaceParam(name, value);
}

bool FiltreCookie::isExclusif() {
	return false;
}

bool FiltreCookie::isBypassingExclusif() {
	return true;
}

bool FiltreCookie::isUriDirectoryValidationRequired() {
	return false;
}

std::string FiltreCookie::getProperty(std::string name, std::string defaultVal) {
	std::string val = config->getParamStr(name, defaultVal);
	return val;
}

Config* FiltreCookie::getConfig() {
	return config;
}
