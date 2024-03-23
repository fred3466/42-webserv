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
	if (uri.find("?", pos) != std::string::npos)
	{
		path = uri.substr(pos, uri.find("?", pos) - pos);
		pos = uri.find("?", pos) + 1;
		query = uri.substr(pos);
	} else
	{
		path = uri.substr(pos);
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
	std::string path = getPath();
	size_t pos = path.rfind(".");
	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	return "";
}

const std::string Uri::getFileName() const
{
	std::string path = getPath();
	size_t pos = path.rfind("/");
	if (pos != std::string::npos)
	{
		return path.substr(pos + 1, path.rfind(".") - pos - 1);
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