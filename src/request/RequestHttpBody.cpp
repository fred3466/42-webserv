#include "RequestHttpBody.h"

RequestHttpBody::RequestHttpBody(char *rawRequest, int rawRequestTotalLen, int bodyLen) {
	if ((!rawRequest) || (rawRequestTotalLen <= bodyLen)) {
		content = new char[0];
		this->len = 0;
		return;
	}

	std::stringstream lines;
	int szCopy = 0;

	szCopy = bodyLen;
	content = new char[szCopy]();
	memset(content, 0, szCopy);
	memcpy(content, rawRequest + (rawRequestTotalLen - bodyLen), szCopy);
	this->len = szCopy;

	if (HARL_LEVEL >= 4) {
		std::string fname = "DBG/_REQUEST_content.txt";
		std::ofstream os(fname.c_str(), std::ios::binary | std::ios::out);
		os.write(content, szCopy);
		os.close();
	}

}

int RequestHttpBody::getLen() {
	return len;
}

void RequestHttpBody::setLen(int len) {
	this->len = len;
}

char* RequestHttpBody::getContent() {
	return content;
}

RequestHttpBody::~RequestHttpBody() {
	delete[] content;
}

RequestHttpBody::RequestHttpBody(const RequestHttpBody &o) : RequestBody(), content(o.content), len(o.len) {
	if (this != &o)
		*this = o;
}

RequestHttpBody& RequestHttpBody::operator=(const RequestHttpBody &o) {
	this->content = o.content;
	this->len = o.len;
	return *this;
}

