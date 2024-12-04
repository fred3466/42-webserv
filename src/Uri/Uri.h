#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "UriValidator.h"
#include "../util/StringUtil.h"

class Uri {
private:
	StringUtil su;
	std::string _uri;
	std::string _path;
	std::string _fileName;
	std::string _fileExt;
	std::string _pathInfo;
	std::string _query;
	bool bDirectory;

public:
	Uri();
	Uri(const std::string &uri);
	~Uri();
	Uri(const Uri &other);
	Uri& operator=(const Uri &other);

	const std::string& getUri() const;
	const std::string& getPath() const;
	const std::string& getPathInfo() const;
	const std::string& getQuery() const;
	const std::string getFileExtension() const;
	const std::string getFileName() const;
	const std::string getFileNameAndExt() const;
	const std::string getPathFileNameAndExt() const;

	void setPath(const std::string &path);
	void setFileName(const std::string &fileName);
	void setFileExt(const std::string &fileExt);
	void setQuery(const std::string &query);
	void setPathInfo(const std::string &pathInfo);
	void setFileNameAndExt(const std::string &fileNameAndExt);
	void dump() const;
	bool isDirectory();
	void updateUriStr();
};

