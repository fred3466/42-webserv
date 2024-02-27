/*
 * ResponseHttp.cpp
 *
 *  Created on: 23 févr. 2024
 *      Author: fbourgue
 */

#include "ResponseHttp.h"

ResponseHttp::ResponseHttp()
{
	// TODO Auto-generated constructor stub

}

ResponseHttp::~ResponseHttp()
{
	// TODO Auto-generated destructor stub
}

void ResponseHttp::setStatusLine(std::string sline)
{
	status_line = sline;
}

void ResponseHttp::setHeader(std::string sline)
{
	header = sline;
}

void ResponseHttp::setBody(std::string sline)
{
	body = sline;
}
std::string ResponseHttp::getStatusLine()
{
	return status_line;
}
std::string ResponseHttp::getHeader()
{
	return header;
}
std::string ResponseHttp::getBody()
{
	return body;
}
int ResponseHttp::getBodyLength()
{
	return bodyLength;
}
void ResponseHttp::setBodyLength(int len)
{
	bodyLength = len;
}
//ResponseHttp::ResponseHttp(const ResponseHttp &other)
//{
//	// TODO Auto-generated constructor stub
//
//}
//
//ResponseHttp& ResponseHttp::operator=(const ResponseHttp &other)
//{
//	// TODO Auto-generated method stub
//
//}

