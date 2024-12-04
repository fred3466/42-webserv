#include "LocationToProcessor.h"

LocationToProcessor::LocationToProcessor() : urlPath(""), extension(""), processor(NULL), host(""), method("") {
}

LocationToProcessor::LocationToProcessor(std::string urlPath, std::string ext, Processor *proc, std::string host, std::string hostAsIpPort, std::string method) {
	extension = ext;
	processor = proc;
	this->urlPath = urlPath;
	this->host = host;
	this->hostAsIpPort = hostAsIpPort;
	this->method = method;
}

LocationToProcessor::~LocationToProcessor() {

	Harl().debug("LocationToProcessor::~LocationToProcessor : kill processor : %s", processor->toString().c_str());

	delete processor;
}

LocationToProcessor::LocationToProcessor(LocationToProcessor &bis) {
	if (this != &bis) {
		extension = bis.extension;
		processor = bis.processor;
		urlPath = bis.urlPath;
		host = bis.host;
		hostAsIpPort = bis.hostAsIpPort;
		method = bis.method;
	}

	*this = bis;
}
LocationToProcessor& LocationToProcessor::operator=(LocationToProcessor &bis) {
	if (this != &bis) {
		extension = bis.extension;
		processor = bis.processor;
		urlPath = bis.urlPath;
		host = bis.host;
		hostAsIpPort = bis.hostAsIpPort;
		method = bis.method;
	}
	return *this;
}

std::string LocationToProcessor::getExtension() {
	return extension;
}

void LocationToProcessor::setExtension(std::string ext) {
	extension = ext;
}

Processor* LocationToProcessor::getProcessor() {
	return processor;
}

void LocationToProcessor::setProcessor(Processor *proc) {
	processor = proc;
}

std::string LocationToProcessor::getUrlPath() {
	return urlPath;
}

void LocationToProcessor::setUrlPath(std::string urlPath) {
	this->urlPath = urlPath;
}

std::string LocationToProcessor::toString() {
	return ("LocationToProcessor : [" + host + "] [" + hostAsIpPort + "] [" + urlPath + "] \t [" + extension + "] \t [" + method + "] \t [" + getProcessor()->toString() + "]").c_str();
}

std::string LocationToProcessor::getHost() {
	return host;
}

std::string LocationToProcessor::getMethod() {
	return method;
}

std::string LocationToProcessor::getHostAsIpPort() {
	return hostAsIpPort;
}

void LocationToProcessor::setHostAsIpPort(std::string hostAsIpPort) {
	this->hostAsIpPort = hostAsIpPort;
}

void LocationToProcessor::setMethod(std::string method) {
	this->method = method;
}

void LocationToProcessor::setHost(std::string host) {
	this->host = host;
}
