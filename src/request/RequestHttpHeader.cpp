#include "RequestHttpHeader.h"

RequestHttpHeader::~RequestHttpHeader()
{
}

RequestHttpHeader::RequestHttpHeader(std::string *rawRequest) : fields()
{
	std::stringstream lines;
	lines.str(rawRequest->c_str());
	char key[2048], val[2048], line[2048];
	bool firstLine = true;

	while (lines)
	{
		lines.getline(line, 2048, '\n');
		std::stringstream lineSs;
		lineSs.str(line);
		std::string lineStr = lineSs.str();

		if (firstLine) //!lineStr.compare(0, 3, "GET"))
		{
			std::string method = lineStr.substr(0, 3);
			this->setMethod(method);
			lineSs.getline(key, 2048, ' ');
			lineSs >> key;
			this->setUri(std::string(key));
			firstLine = false;
		} else
		{
			lineSs.getline(val, 2048, '\n');
			this->addField(std::string(val));
		}
	}
	//cookie set plusieurs fois ?
	//CookieFactory().build(this);
}

std::string RequestHttpHeader::getFieldValue(std::string fieldName) const
{
	std::string ret = "";
	StringUtil su = StringUtil();
	std::string fieldNameUpper = su.strUpperCase(fieldName);

	for (std::list<std::string>::const_iterator ite = fields.begin(); ite != fields.end();
			ite++)
	{
		std::string rawField = *ite;
		int nbSeparatorsToProcess = 1;
		std::vector<std::string> toks = su.tokenize(rawField, ':', nbSeparatorsToProcess);
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		name = su.strUpperCase(su.trim(name));
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		val = su.trim(val);
		if (name == fieldNameUpper)
			return val;
	}
	return ret;
}

std::string RequestHttpHeader::toString()
{
	std::string ret = "";
	ret += "RequestHttpHeader : Method : [" + getMethod() + "]\t[" + getUri() + "]\t[" + getVersion() + "]\n";

	StringUtil su = StringUtil();
	ret += su.dedoublonne(su.fromListToString(fields), "\n");
	return ret;
}
void RequestHttpHeader::addField(std::string f)
{
	if (!f.empty())
		fields.push_back(f);
}

const std::list<std::string>& RequestHttpHeader::getFields() const
{
	return fields;
}

const std::string& RequestHttpHeader::getMethod() const
{
	return method;
}

void RequestHttpHeader::setMethod(const std::string &m)
{
	method = m;
}

const std::string& RequestHttpHeader::getVersion() const
{
	return version;
}

void RequestHttpHeader::setVersion(const std::string &v)
{
	version = v;
}

Cookie RequestHttpHeader::getCookie(const std::string &cookieName)
{
	return cookieHelper.getCookie(cookies, cookieName);
}

bool RequestHttpHeader::addCookie(const Cookie &cookie)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.addCookie(cookies, cookie);
	if (cookies.size() > i)
		ret = true;
	return ret;
}

bool RequestHttpHeader::removeCookie(const std::string &cookieName)
{
	bool ret = false;
	int i = cookies.size();
	cookies = cookieHelper.removeCookie(cookies, cookieName);
	if (cookies.size() < i)
		ret = true;
	return ret;
}

std::string RequestHttpHeader::getCookieString()
{
	return cookieHelper.getCookieString(cookies);
}

const std::string& RequestHttpHeader::getUri() const
{
	return uri.getUri();
}

void RequestHttpHeader::setUri(const std::string &u)
{
	uri = Uri(u);
}

const std::string& RequestHttpHeader::getQueryString() const
{
	return uri.getQuery();
}

const std::string RequestHttpHeader::getFileExtension() const
{
	return uri.getFileExtension();
}

const std::string RequestHttpHeader::getFileName() const
{
	return uri.getFileName();
}

const std::string& RequestHttpHeader::getPath() const
{
	return uri.getPath();
}