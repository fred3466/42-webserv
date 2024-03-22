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
	
	std::string scheme;
	scheme = uri.substr(0, uri.find("://"));
	size_t pos = uri.find("://") + 3;
	if (UriValidator().checkScheme(scheme))
	{
		setScheme(scheme);
	} else
	{
		Harl().error("Invalid scheme: " + scheme);
	}

	std::string user = "";
	if (uri.find("@", pos) != std::string::npos)
	{
		user = uri.substr(pos, uri.find(":", pos) - pos);
		pos = uri.find(":", pos) + 1;
	}
	setUser(user);

	std::string password = "";
	if (uri.find("@", pos) != std::string::npos)
	{
		password = uri.substr(pos, uri.find("@", pos) - pos);
		pos = uri.find("@", pos) + 1;
	}
	setPassword(password);

	std::string host;
	std::string port = "";
	if (uri.find(":", pos) != std::string::npos)
	{
		host = uri.substr(pos, uri.find(":", pos) - pos);
		pos = uri.find(":", pos) + 1;
		port = uri.substr(pos, uri.find("/", pos) - pos);
		pos = uri.find("/", pos);
	} else
	{
		host = uri.substr(pos, uri.find("/", pos) - pos);
		pos = uri.find("/", pos);
	}
	setHost(host);
	setPort(port);

	std::string path;
	std::string query = "";
	std::string fragment = "";
	if (uri.find("?", pos) != std::string::npos)
	{
		path = uri.substr(pos, uri.find("?", pos) - pos);
		pos = uri.find("?", pos) + 1;
		if (uri.find("#", pos) != std::string::npos)
		{
			query = uri.substr(pos, uri.find("#", pos) - pos);
			pos = uri.find("#", pos) + 1;
			fragment = uri.substr(pos);
		} else
		{
			query = uri.substr(pos);
		}
	} else if (uri.find("#", pos) != std::string::npos)
	{
		path = uri.substr(pos, uri.find("#", pos) - pos);
		pos = uri.find("#", pos) + 1;
		fragment = uri.substr(pos);
	} else
	{
		path = uri.substr(pos);
	}
	path = UriValidator().formatPath(path);
	setPath(path);
	setQuery(query);
	setFragment(fragment);
}

std::string Uri::getUri() const
{
	return _uri;
}

std::string Uri::getScheme() const
{
	return _scheme;
}

std::string Uri::getUser() const
{
	return _user;
}

std::string Uri::getPassword() const
{
	return _password;
}

std::string Uri::getHost() const
{
	return _host;
}

std::string Uri::getPort() const
{
	return _port;
}

std::string Uri::getPath() const
{
	return _path;
}

std::string Uri::getQuery() const
{
	return _query;
}

std::string Uri::getFragment() const
{
	return _fragment;
}

std::string Uri::getUrl() const
{
	std::string url = getScheme() + "://";
	if (getUser().length() > 0)
	{
		url += getUser();
		if (getPassword().length() > 0)
		{
			url += ":" + getPassword();
		}
		url += "@";
	}
	url += getHost();
	if (getPort().length() > 0)
	{
		url += ":" + getPort();
	}
	url += getPath();
	if (getQuery().length() > 0)
	{
		url += "?" + getQuery();
	}
	if (getFragment().length() > 0)
	{
		url += "#" + getFragment();
	}
	return url;
}

std::string Uri::getFileExtension() const
{
	std::string path = getPath();
	size_t pos = path.rfind(".");
	if (pos != std::string::npos)
	{
		return path.substr(pos);
	}
	return "";
}

std::string Uri::getFileName() const
{
	std::string path = getPath();
	size_t pos = path.rfind("/");
	if (pos != std::string::npos)
	{
		return path.substr(pos + 1);
	}
	return "";
}

void Uri::setUri(const std::string &uri)
{
	_uri = uri;
}

void Uri::setScheme(const std::string &scheme)
{
	_scheme = scheme;
}

void Uri::setUser(const std::string &user)
{
	_user = user;
}	

void Uri::setPassword(const std::string &password)
{
	_password = password;
}

void Uri::setHost(const std::string &host)
{
	_host = host;
}

void Uri::setPort(const std::string &port)
{
	_port = port;
}

void Uri::setPath(const std::string &path)
{
	_path = path;
}

void Uri::setQuery(const std::string &query)
{
	_query = query;
}

void Uri::setFragment(const std::string &fragment)
{
	_fragment = fragment;
}

void Uri::dump() const
{
	Harl().debug("Uri: " + getUri());
	Harl().debug("Url: " + getUrl());
	Harl().debug("Scheme: " + getScheme());
	Harl().debug("User: " + getUser());
	Harl().debug("Password: " + getPassword());
	Harl().debug("Host: " + getHost());
	Harl().debug("Port: " + getPort());
	Harl().debug("Path: " + getPath());
	Harl().debug("Query: " + getQuery());
	Harl().debug("Fragment: " + getFragment());
	Harl().debug("File extension: " + getFileExtension());
	Harl().debug("File name: " + getFileName());
}