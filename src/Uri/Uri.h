#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include "UriValidator.h"

class Uri
{
private:
	std::string _uri;
	//	std::string _scheme;
	std::string _path;
	std::string _fileName;
	std::string _fileExt;
	std::string _pathInfo;
	std::string _query;
	public:
	Uri();
	Uri(const std::string &uri);
	~Uri();
	const std::string& getUri() const;
	//	const std::string getScheme() const;
	const std::string& getPath() const;
	const std::string& getPathInfo() const;
	const std::string& getQuery() const;
	const std::string getFileExtension() const;
	const std::string getFileName() const;
	void setUri(const std::string &uri);
	void setPath(const std::string &path);
	void setQuery(const std::string &query);
	void setPathInfo(const std::string &pathInfo);
	void dump() const;
};

// validator:
// - check if scheme == "http"
// - check and format path to a correct
