#include "ResponseHttp.h"

ResponseHttp::ResponseHttp(ResponseHeader *head) :
		bodyLength(0), bodyBin(NULL)
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

void ResponseHttp::setBody(std::string sline)
{
	body = sline;
}
std::string ResponseHttp::getStatusLine()
{
	return header->getStatusLine();
}
ResponseHeader* ResponseHttp::getHeader()
{
	return header;
}
std::string ResponseHttp::getBody()
{
	return body;
}
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

