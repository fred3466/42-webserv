#include "ResponseHttp.h"

ResponseHttp::ResponseHttp(ResponseHeader *head) : bodyBin(NULL), bodyLength(0), totalLength(0), flagCgi(false), flagError(false) //,  error(NULL)
{
	header = head;
}

ResponseHttp::~ResponseHttp()
{
}

void ResponseHttp::setStatusLine(std::string sline)
{
	// TODO decommenter setStatusLine dans ResponseHeader
	// header->setStatusLine(sline);
}

// void ResponseHttp::setBody(std::string sline)
//{
//	body = sline;
// }
std::string ResponseHttp::getStatusLine()
{
	return header->getStatusLine();
}
ResponseHeader *ResponseHttp::getHeader()
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

char *ResponseHttp::getBodyBin()
{
	return bodyBin;
}

bool ResponseHttp::isCgi()
{
	return flagCgi;
}

// TODO @Anastasia : virer le bool et tester HttpError.code
bool ResponseHttp::isError()
{
	return flagError;
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

//	TODO @Anastasia : temporaire, à virer après intégration de HttpError
void ResponseHttp::setIsError(bool isError)
{
	this->flagError = isError;
}

void ResponseHttp::setHttpError(HttpError *error)
{
	this->error = error;
	this->flagError = true;

	std::ostringstream statusLine;
	statusLine << "HTTP/1.1 " << error->getCode() << " " << error->getDescription();
	// TODO remettre setStatusLine
	// this->header->setStatusLine(statusLine.str());
}

int ResponseHttp::getErrorCodeTmp()
{
	return errorCodeTmp;
}

void ResponseHttp::setErrorCodeTmp(int errorCode)
{
	errorCodeTmp = errorCode;
}
