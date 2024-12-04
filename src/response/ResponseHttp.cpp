#include "ResponseHttp.h"
#include "../Harl.h"

ResponseHttp::ResponseHttp(ResponseHeader *head) :
		bodyBin(NULL), bodyLength(0), totalLength(0), flagCgi(false), flagRedirect(false),
		errorCodeTmp(200), bNeedFurtherProcessing(false), fd(-1) {
	header = head;
}

ResponseHttp::~ResponseHttp() {
	if (header) {
		Harl().debug("ResponseHttp::~ResponseHttp() xoxoxoxoxoxoxoxoxxoxoxoxoxoxoxoxoxoxoxo delete header");
		delete header;
		header = NULL;
	}
	if (bodyBin) {
		Harl().debug("ResponseHttp::~ResponseHttp() xoxoxoxoxoxoxoxoxxoxoxoxoxoxoxoxoxoxoxo delete bodyBin");
		delete[] bodyBin;
		bodyBin = NULL;
	}
}

void ResponseHttp::setStatusLine(std::string sline) {
	header->setStatusLine(sline);
}

std::string ResponseHttp::getStatusLine() {
	return header->getStatusLine();
}
ResponseHeader* ResponseHttp::getHeader() {
	return header;
}
int ResponseHttp::getTotalLength() {
	return totalLength;
}
void ResponseHttp::setTotalLength(int len) {
	totalLength = len;
}
int ResponseHttp::getBodyLength() {
	return bodyLength;
}
void ResponseHttp::setBodyLength(int len) {
	bodyLength = len;
}
void ResponseHttp::setBodyBin(char *bytess) {
	this->bodyBin = bytess;
}

char* ResponseHttp::getBodyBin() {
	return bodyBin;
}

bool ResponseHttp::isCgi() {
	return flagCgi;
}

bool ResponseHttp::isRedirect() {
	return flagRedirect;
}

void ResponseHttp::setCgi(bool cgi) {
	flagCgi = cgi;
}

void ResponseHttp::setIsRedirect(bool isRedirect) {
	this->flagRedirect = isRedirect;
}

int ResponseHttp::getErrorCodeTmp() {
	return errorCodeTmp;
}

bool ResponseHttp::isBNeedFurtherProcessing()
{
	return bNeedFurtherProcessing;
}

int ResponseHttp::getFd()
{
	return fd;
}

void ResponseHttp::setFd(int fd)
		{
	this->fd = fd;
}

void ResponseHttp::setBNeedFurtherProcessing(bool bNeedFurtherProcessing)
		{
	this->bNeedFurtherProcessing = bNeedFurtherProcessing;
}

void ResponseHttp::setErrorCodeTmp(int errorCode) {
	errorCodeTmp = errorCode;
}
