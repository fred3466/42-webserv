#include "ResponseHttp.h"

// TODO En faire une Coplien
ResponseHttp::ResponseHttp(ResponseHeader *head) : bodyBin(NULL), bodyLength(0), totalLength(0), flagCgi(false), flagRedirect(false), error(), errorCodeTmp(200)
{
	header = head;
}

ResponseHttp::~ResponseHttp()
{
}

void ResponseHttp::setStatusLine(std::string sline)
{
	header->setStatusLine(sline);
}

// void ResponseHttp::setBody(std::string sline)
//{
//	body = sline;
// }
std::string ResponseHttp::getStatusLine()
{
	return header->getStatusLine();
}
ResponseHeader* ResponseHttp::getHeader()
{
	return header;
}
// std::string ResponseHttp::getBody()
//{
//	return body;
// }
int ResponseHttp::getTotalLength()
{
	return totalLength;
}
void ResponseHttp::setTotalLength(int len)
{
	totalLength = len;
}
int ResponseHttp::getBodyLength()
{
	return bodyLength;
}
void ResponseHttp::setBodyLength(int len)
{
	bodyLength = len;
}
void ResponseHttp::setBodyBin(char *bytess)
{
	this->bodyBin = bytess;
}

char* ResponseHttp::getBodyBin()
{
	return bodyBin;
}

bool ResponseHttp::isCgi()
{
	return flagCgi;
}

// TODO @Anastasia : virer le bool et tester HttpError.code
bool ResponseHttp::isRedirect()
{
	return flagRedirect;
}

// HttpError* ResponseHttp::getError()
//{
//	return error;
// }

void ResponseHttp::setCgi(bool cgi)
{
	flagCgi = cgi;
}

// void ResponseHttp::setError(HttpError *error)
//{
//	this->error = error;
//}

void ResponseHttp::setIsRedirect(bool isRedirect)
{
	this->flagRedirect = isRedirect;
}

void ResponseHttp::setHttpError(HttpError *error)
{
	this->error = error;
	this->flagRedirect = error->getCode() != 200;

	std::ostringstream statusLine;
	statusLine << "HTTP/1.1 " << error->getCode() << " " << error->getDescription();
	this->header->setStatusLine(statusLine.str());
	setErrorCodeTmp(error->getCode());
}

HttpError* ResponseHttp::getHttpError()
{
	return error;
}

int ResponseHttp::getErrorCodeTmp()
{
	return errorCodeTmp;
}

void ResponseHttp::setErrorCodeTmp(int errorCode)
{
	errorCodeTmp = errorCode;
}
