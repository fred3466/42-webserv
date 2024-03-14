#include "RequestHttpHeader.h"

RequestHttpHeader::~RequestHttpHeader()
{
}

RequestHttpHeader::RequestHttpHeader(std::string *rawRequest) :
		statusLine(), fields()
{
	std::stringstream lines;
	lines.str(rawRequest->c_str());
	char key[2048], val[2048], line[2048];
	while (lines)
	{
		lines.getline(line, 2048, '\n');
		std::stringstream lineSs;
		lineSs.str(line);
		std::string lineStr = lineSs.str();
		if (!lineStr.compare(0, 3, "GET"))
		{
			this->setMethod("GET");
			lineSs.getline(key, 2048, ' ');
			lineSs >> key;
			this->setUri(std::string(key));
		} else
		{
			lineSs.getline(val, 2048, '\n');
			this->addField(std::string(val));
		}
	}
}

std::string RequestHttpHeader::getFieldValue(std::string fieldName) const
{
	std::string ret = "";
	StringUtil su = StringUtil();

	for (std::list<std::string>::const_iterator ite = fields.begin(); ite != fields.end();
			ite++)
	{
		std::string rawField = *ite;
		std::vector<std::string> toks = su.tokenize(rawField, ':');
		std::string name = su.getNthTokenIfExists(toks, 0, "");
		std::string val = su.getNthTokenIfExists(toks, 1, "");
		if (name == fieldName)
			return val;
	}
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

const std::string& RequestHttpHeader::getUri() const
{
	return uri;
}

void RequestHttpHeader::setUri(const std::string &u)
{
	uri = u;
}

const std::string& RequestHttpHeader::getVersion() const
{
	return version;
}

void RequestHttpHeader::setVersion(const std::string &v)
{
	version = v;
}
