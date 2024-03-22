#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include "UriValidator.h"

class Uri
{
private:
	std::string _uri;
	std::string _scheme;
	std::string _user;
	std::string _password;
	std::string _host;
	std::string _port;
	std::string _path;
	std::string _query;
	std::string _fragment;
public:
	Uri();
	Uri(const std::string &uri);
	~Uri();
	std::string getUri() const;
	std::string getScheme() const;
	std::string getUser() const;
	std::string getPassword() const;
	std::string getHost() const;
	std::string getPort() const;
	std::string getPath() const;
	std::string getQuery() const;
	std::string getFragment() const;
	std::string getUrl() const;
	std::string getFileExtension() const;
	std::string getFileName() const;
	void setUri(const std::string &uri);
	void setScheme(const std::string &scheme);
	void setUser(const std::string &user);
	void setPassword(const std::string &password);
	void setHost(const std::string &host);
	void setPort(const std::string &port);
	void setPath(const std::string &path);
	void setQuery(const std::string &query);
	void setFragment(const std::string &fragment);
	void dump() const;
};

// validator:
// - check if scheme == "http"
// - check and format path to a correct