#include "ResponseHttpHeader.h"

ResponseHttpHeader::ResponseHttpHeader() : fields(), su()
{
}

ResponseHttpHeader::~ResponseHttpHeader()
{
}
int ResponseHttpHeader::getFieldAsInt(std::string param, int intDefault)
{
	try
	{
		std::string res = fields.at(param);
		if (!res.empty())
		{
			int resInt = su.intFromStr(res);
			return resInt;
		}
	}
	catch (const std::out_of_range &oor)
	{
//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return intDefault;
}

std::string ResponseHttpHeader::getFieldAsStr(std::string param, std::string stringDefault)
{
	try
	{
		std::string res = fields.at(param);
		if (!res.empty())
		{
			return res;
		}
	}
	catch (const std::out_of_range &oor)
	{
//		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return stringDefault;
}

void ResponseHttpHeader::addField(std::string headerName, std::string headerValue)
{
	if (headerValue == "" || headerName == "")
		return;
	fields[headerName] = headerValue;
}

void ResponseHttpHeader::addNoReplaceField(std::string headerName, std::string headerValue)
{
	if (headerValue == "" || headerName == "")
		return;

	try
	{
		fields.at(headerName);
	} catch (const std::out_of_range &oor)
	{
		fields[headerName] = headerValue;
	}
}

std::list<std::string>* ResponseHttpHeader::getFields()
{
//	TODO new par ici
	std::list<std::string> *l = new std::list<std::string>();
	l->resize(fields.size());

	for (std::map<std::string, std::string>::iterator ite = fields.begin(); ite != fields.end(); ite++)
	{
		std::string keyVal = ite->first + ": " + ite->second + "\r\n";
		;
		l->push_back(keyVal);
	}
	return l;
}

std::string ResponseHttpHeader::getStatusLine()
{
	return statusLine;
}

void ResponseHttpHeader::setStatusLine(std::string statusLine)
{
	this->statusLine = statusLine;
}

Cookie ResponseHttpHeader::getCookie(const std::string &cookieName)
{
	return cookieHelper.getCookie(cookies, cookieName);
}

bool ResponseHttpHeader::addCookie(const Cookie &cookie)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.addCookie(cookies, cookie);
	if ((int) cookies.size() > i)
		ret = true;
	return ret;
}

bool ResponseHttpHeader::removeCookie(const std::string &cookieName)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.removeCookie(cookies, cookieName);
	if ((int) cookies.size() < i)
		ret = true;
	return ret;
}

std::string ResponseHttpHeader::getCookieString()
{
	return cookieHelper.getCookieStringResponse(cookies);
}

void ResponseHttpHeader::setErrorCodeTmp(int errorCode)
{
	this->errorCode = errorCode;
}
