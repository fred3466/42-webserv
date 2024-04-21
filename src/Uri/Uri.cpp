#include "Uri.h"
#include "../Harl.h"

Uri::Uri()
{
}

Uri::~Uri()
{
}

// Uri::Uri(const std::string &uri)
// {
// 	setUri(uri);
// ///cgi-perl/form-submit.pl/path/info?q=fred&titi=toto

// 	std::string path = "";
// 	std::string fileName = "";
// 	std::string fileExt = "";
// 	std::string pathInfo = "";
// 	std::string query = "";

// 	size_t posQM = uri.find("?");
// //	bool bGotQuery = posQM != std::string::npos;
// 	size_t posFirstDotFromStart = uri.find(".");
// 	size_t posFirstSlashAfterDot = uri.find("/", posFirstDotFromStart + 1);
// 	size_t posLastSlashBeforeDot = uri.rfind("/", posFirstDotFromStart);
// //	size_t posLastSlashAfterDot = uri.rfind("/", posFirstDotFromStart);

// //	path
// 	path = uri.substr(0, posLastSlashBeforeDot + 1);
// 	int pathLength = path.size();

// //	fileName
// 	fileName = uri.substr(pathLength, posFirstDotFromStart - pathLength);

// //	fileExt
// 	size_t endExt = posFirstSlashAfterDot != std::string::npos ? posFirstSlashAfterDot : posQM;
// 	fileExt = uri.substr(posFirstDotFromStart, endExt - posFirstDotFromStart);

// 	if (posFirstSlashAfterDot != std::string::npos)
// 	{
// 		size_t endPathInfo = posQM != std::string::npos ? posQM : posFirstSlashAfterDot;
// 		pathInfo = uri.substr(posFirstSlashAfterDot, endPathInfo - posFirstSlashAfterDot);
// 	}

// 	if (posQM != std::string::npos)
// 		query = uri.substr(posQM + 1);

// 	_fileName = fileName;
// 	_fileExt = fileExt;

// 	setPath(path);
// 	setPathInfo(pathInfo);
// 	setQuery(query);
// }

Uri::Uri(const std::string &uri)
{
	setUri(uri);
	std::string path = "";
	std::string fileName = "";
	std::string fileExt = "";
	std::string pathInfo = "";
	std::string query = "";

	size_t posQM = uri.find("?");
	size_t posFirstDotFromStart = uri.find(".");
	size_t posFirstSlashAfterDot = uri.find("/", posFirstDotFromStart + 1);
	size_t posLastSlashBeforeDot = uri.rfind("/", posFirstDotFromStart);

	// Path
	if (posLastSlashBeforeDot != std::string::npos)
	{
		path = uri.substr(0, posLastSlashBeforeDot + 1);
	}

	// fileName
	if (posLastSlashBeforeDot != std::string::npos && posFirstDotFromStart != std::string::npos && posFirstDotFromStart > posLastSlashBeforeDot)
	{
		fileName = uri.substr(posLastSlashBeforeDot + 1, posFirstDotFromStart - posLastSlashBeforeDot - 1);
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

const std::string &Uri::getUri() const
{
	return _uri;
}

const std::string &Uri::getPath() const
{
	return _path;
}

const std::string &Uri::getQuery() const
{
	return _query;
}

const std::string &Uri::getPathInfo() const
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
