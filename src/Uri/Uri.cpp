#include "Uri.h"
#include "../Harl.h"

Uri::Uri() : su()
{
	_uri = "";
	_path = "";
	_fileName = "";
	_fileExt = "";
	_pathInfo = "";
	_query = "";
	bDirectory = false;
}

Uri::~Uri()
{
}

Uri::Uri(const std::string &uri) : su()
{
	setUri(uri);
	std::string path = "";
	std::string fileName = "";
	std::string fileExt = "";
	std::string pathInfo = "";
	std::string query = "";
	this->bDirectory = false;

	size_t posQM = uri.find("?");
	size_t posFirstDotFromStart = uri.find(".");
	size_t posFirstSlashAfterDot = uri.find("/", posFirstDotFromStart + 1);
	size_t posLastSlashBeforeDot = uri.rfind("/", posFirstDotFromStart);

	// fileName
	if (posLastSlashBeforeDot != std::string::npos && posFirstDotFromStart != std::string::npos && posFirstDotFromStart > posLastSlashBeforeDot)
	{
		fileName = uri.substr(posLastSlashBeforeDot + 1, posFirstDotFromStart - posLastSlashBeforeDot - 1);
	}

	// Path
	if (fileName == "")
	{
		path = uri;
		this->bDirectory = true;
	} else
	{
		if (posLastSlashBeforeDot != std::string::npos)
		{
			path = uri.substr(0, posLastSlashBeforeDot + 1);
		}

		// fileExt
		if (posFirstDotFromStart != std::string::npos)
		{
			size_t endExt = (posFirstSlashAfterDot != std::string::npos) ? posFirstSlashAfterDot : ((posQM != std::string::npos) ? posQM : uri.length());
			fileExt = uri.substr(posFirstDotFromStart, endExt - posFirstDotFromStart);
		}

		// pathInfo
		if (posFirstSlashAfterDot != std::string::npos)
		{
			size_t endPathInfo = (posQM != std::string::npos) ? posQM : uri.length();
			pathInfo = uri.substr(posFirstSlashAfterDot, endPathInfo - posFirstSlashAfterDot);
		}
	}

	// query
	if (posQM != std::string::npos)
	{
		query = uri.substr(posQM + 1);
	}

	_fileName = fileName;
	_fileExt = fileExt;
	setPath(path);
	setPathInfo(pathInfo);
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

const std::string& Uri::getPathInfo() const
{
	return _pathInfo;
}

const std::string Uri::getFileExtension() const
{
	return _fileExt;
}

const std::string Uri::getFileName() const
{
	return _fileName;
}

void Uri::setUri(const std::string &uri)
{
	_uri = uri;
}

void Uri::setPath(const std::string &path)
{
	_path = path;
}

void Uri::setFileName(const std::string &fileName)
{
	_fileName = fileName;
}

void Uri::setFileExt(const std::string &fileExt)
{
	_fileExt = fileExt;
}

std::string Uri::getFileNameAndExt()
{
	return _fileName + _fileExt;
}

void Uri::setFileNameAndExt(const std::string &fileNameAndExt)
{
	std::vector<std::string> toks = su.tokenize(fileNameAndExt, '.');
	if (toks.empty())
		_fileName = fileNameAndExt;
	else
	{
		_fileExt = "." + toks.at(toks.size() - 1);
		_fileName = "";
		for (size_t i = 0; i < toks.size() - 1; i++)
		{
			_fileName += toks.at(i);
		}
	}
}

void Uri::setQuery(const std::string &query)
{
	_query = query;
}

void Uri::setPathInfo(const std::string &pathInfo)
{
	_pathInfo = pathInfo;
}

void Uri::dump() const
{
	Harl().debug("Uri: " + getUri());
	Harl().debug("Path: " + getPath());
	Harl().debug("Pathinfo: " + getPathInfo());
	Harl().debug("Query: " + getQuery());
	Harl().debug("File extension: " + getFileExtension());
	Harl().debug("File name: " + getFileName());
}

bool Uri::isDirectory()
{
	return bDirectory;
}

Uri::Uri(Uri const &other) : bDirectory(other.bDirectory)
{
	su = other.su;
	_uri = other._uri;
	_path = other._path;
	_fileName = other._fileName;
	_fileExt = other._fileExt;
	_pathInfo = other._pathInfo;
	_query = other._query;
	bDirectory = other.bDirectory;
	if (this != &other)
		*this = other;
}

Uri& Uri::operator=(Uri const &other)
{
	su = other.su;
	_uri = other._uri;
	_path = other._path;
	_fileName = other._fileName;
	_fileExt = other._fileExt;
	_pathInfo = other._pathInfo;
	_query = other._query;
	bDirectory = other.bDirectory;
	return *this;
}

void Uri::updateUriStr()
{
	Uri uriTmp(_path + _fileName + _fileExt + _pathInfo + _query);
	bDirectory = uriTmp.bDirectory;
	_uri = uriTmp._uri;
}
