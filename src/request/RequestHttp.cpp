#include "RequestHttp.h"
#include "../Harl.h"

RequestHttp::~RequestHttp() {
	if (header) {
		Harl().debug("RequestHttp::~RequestHttp() xoxoxoxoxoxoxoxoxxoxoxoxoxoxoxoxoxoxoxo delete header");
		delete header;
		header = NULL;
	}
	if (body) {
		Harl().debug("RequestHttp::~RequestHttp() xoxoxoxoxoxoxoxoxxoxoxoxoxoxoxoxoxoxoxo delete body");
		delete body;
		body = NULL;
	}
}

RequestHttp::RequestHttp(RequestHeader *head, RequestBody *body)
: fdClient(-1), bConnectionKeepAlive(false), bResponseAlreadySent(false) {
	StringUtil su = StringUtil();
	header = head;
	this->body = body;

	std::string hostPort = getHeaderFieldValue("Host");
	std::vector<std::string> list = su.tokenize(hostPort, ':');
	host = su.getNthTokenIfExists(list, 0, "");
	port = su.intFromStr(su.getNthTokenIfExists(list, 1, "80"));

	std::string connectionVal = header->getFieldValue("Connection");
	bConnectionKeepAlive = StringUtil().isStrictlyEqual(connectionVal, "keep-alive");
}

bool RequestHttp::isResponseAlreadySent()
{
	return bResponseAlreadySent;
}

void RequestHttp::setResponseAlreadySent(bool bResponseAlreadySent)
		{
	this->bResponseAlreadySent = bResponseAlreadySent;
}

RequestHeader* RequestHttp::getHeader() const {
	return header;
}

const std::string RequestHttp::getFileExtension() const {
	return header->getFileExtension();
}

const std::string RequestHttp::getFileName() const {
	return header->getFileName();
}

const std::string RequestHttp::getHost() {
	return host;
}

const std::string& RequestHttp::getPath() {
	return header->getPath();
}

const std::string& RequestHttp::getQueryString() const {
	return header->getQueryString();
}

std::string RequestHttp::getHeaderFieldValue(std::string fieldName) const {
	std::string fieldValue = header->getFieldValue(fieldName);
	return fieldValue;
}

void RequestHttp::addField(std::string rawField) const {
	getHeader()->addField(rawField);
}

const std::list<std::string>& RequestHttp::getFields() const {
	return header->getFields();
}

const Uri& RequestHttp::getUri() const {
	return header->getUri();
}

const std::string& RequestHttp::getMethod() const {
	return header->getMethod();
}

int RequestHttp::getFdClient() const {
	return fdClient;
}
void RequestHttp::setFdClient(int fd) {
	fdClient = fd;
}

void RequestHttp::setBody(RequestBody *body) {
	this->body = body;
}

RequestBody* RequestHttp::getBody() {
	return body;
}

bool RequestHttp::isConnectionKeepAlive() throw (char*) {
	return bConnectionKeepAlive;
}

int RequestHttp::getPort() {
	return port;
}

std::string RequestHttp::getRemoteIpPort() {

	return remoteIpPort;
}

std::string RequestHttp::getUrlParam(std::string paramName) {
	std::string queryString = getQueryString();
	StringUtil su;
	std::vector<std::string> params = su.tokenize(queryString, '&');
	for (size_t i = 0; i < params.size(); ++i) {
		std::vector<std::string> key_value = su.tokenize(params[i], '=');
		if (key_value.size() == 2 && key_value[0] == paramName) {
			return key_value[1]; // Return the value associated with the parameter name
		}
	}
	return ""; // Return empty if the parameter is not found
}

