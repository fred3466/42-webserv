#include "Uri.h"
#include "../Harl.h"

Uri::Uri()
{
}

Uri::~Uri()
{
}

Uri::Uri(const std::string &uri)
{
	setUri(uri);

	std::string path;
	std::string query = "";
	size_t pos = 0;
	size_t posQM = uri.find("?");
	size_t lastSlashBeforeQM = uri.rfind("/", posQM);

	if (posQM != std::string::npos)
	{
//		path = uri.substr(pos, uri.find("?", pos) - pos);
		path = uri.substr(lastSlashBeforeQM);
		query = uri.substr(posQM);
	} else
	{
//		path = uri.substr(pos);
		path = uri.substr(pos, lastSlashBeforeQM);
	}
	path = UriValidator().formatPath(path);
	setPath(path);
	setQuery(query);
}

const std::string& Uri::getUri() const
{
	return _uri;
}

const std::string& Uri::getPath() const
{
	return _path;
}

const std::string& Uri::getQuery() const
{
	return _query;
}

const std::string Uri::getFileExtension() const
{
	std::string path = _uri;
	size_t pos = path.rfind(".");
	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	return "";
}

const std::string Uri::getFileName() const
{
	size_t pos = _uri.rfind("/");
	if (pos != std::string::npos)
	{
		return _uri.substr(pos + 1, _uri.rfind(".") - pos - 1);
	}
	return "";
}

void Uri::setUri(const std::string &uri)
{
	_uri = uri;
}

void Uri::setPath(const std::string &path)
{
	_path = path;
}

void Uri::setQuery(const std::string &query)
{
	_query = query;
}

void Uri::dump() const
{
	Harl().debug("Uri: " + getUri());
	Harl().debug("Path: " + getPath());
	Harl().debug("Query: " + getQuery());
	Harl().debug("File extension: " + getFileExtension());
	Harl().debug("File name: " + getFileName());
}
