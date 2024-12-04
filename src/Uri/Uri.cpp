#include "Uri.h"
#include "../Harl.h"

Uri::Uri() : su() {
	_uri = "";
	_path = "";
	_fileName = "";
	_fileExt = "";
	_pathInfo = "";
	_query = "";
	bDirectory = false;
}

Uri::~Uri() {
}

Uri::Uri(const std::string &uri) : su() {
	std::string path = "";
	std::string fileName = "";
	std::string fileExt = "";
	std::string pathInfo = "";
	std::string query = "";
	this->bDirectory = false;

	size_t posQM = uri.find("?");
	size_t posFirstDotFromStart = uri.find(".");
	if (posFirstDotFromStart > posQM)
		posFirstDotFromStart = posQM;

	size_t posFirstSlashAfterDot = uri.find("/", posFirstDotFromStart + 1);
	if (posFirstSlashAfterDot > posQM)
		posFirstSlashAfterDot = posQM;

	size_t posLastSlashBeforeDot = uri.rfind("/", posFirstDotFromStart);
	if (posLastSlashBeforeDot > posQM)
		posLastSlashBeforeDot = posQM;

	// fileName
	if (posFirstDotFromStart == posQM) {
		fileName = "";
	} else if (posLastSlashBeforeDot != std::string::npos && posFirstDotFromStart != std::string::npos && posFirstDotFromStart > posLastSlashBeforeDot) {
		fileName = uri.substr(posLastSlashBeforeDot + 1, posFirstDotFromStart - posLastSlashBeforeDot - 1);
	}

// Path
	if (fileName == "") {
		path = uri.substr(0, posQM);
		this->bDirectory = true;
	} else {
		if (posLastSlashBeforeDot != std::string::npos) {
			path = uri.substr(0, posLastSlashBeforeDot + 1);
		}

		// fileExt
		if (posFirstDotFromStart != std::string::npos) {
			size_t endExt = (posFirstSlashAfterDot != std::string::npos) ? posFirstSlashAfterDot : ((posQM != std::string::npos) ? posQM : uri.length());
			fileExt = uri.substr(posFirstDotFromStart, endExt - posFirstDotFromStart);
		}

// pathInfo
		if (posFirstSlashAfterDot != std::string::npos) {
			size_t endPathInfo = (posQM != std::string::npos) ? posQM : uri.length();
			pathInfo = uri.substr(posFirstSlashAfterDot, endPathInfo - posFirstSlashAfterDot);
		}
	}

	// query
	if (posQM != std::string::npos) {
		query = uri.substr(posQM + 1);
	}

	_fileName = su.trim(fileName);
	_fileExt = su.trim(fileExt);

	if ((!path.empty()) && (path != "/") && (path.at(path.length() - 1) == '/')) {
		path.resize(path.length() - 1);
	}
	_path = su.trim(path);
	_pathInfo = su.trim(pathInfo);
	_query = su.trim(query);
	_uri = uri;
}

const std::string& Uri::getUri() const {
	return _uri;
}

const std::string& Uri::getPath() const {
	return _path;
}

const std::string& Uri::getQuery() const {
	return _query;
}

const std::string& Uri::getPathInfo() const {
	return _pathInfo;
}

const std::string Uri::getFileExtension() const {
	return _fileExt;
}

const std::string Uri::getFileName() const {
	return _fileName;
}

const std::string Uri::getFileNameAndExt() const {
	return _fileName + _fileExt;
}

const std::string Uri::getPathFileNameAndExt() const {
	std::string slash = "";
	if ((!_path.empty()) && _path.at(_path.size() - 1) != '/')
		slash = "/";
	return _path + slash + _fileName + _fileExt;
}

void Uri::setPath(const std::string &path) {
	_path = su.trim(path);
	updateUriStr();
}

void Uri::setFileName(const std::string &fileName) {
	_fileName = su.trim(fileName);
	updateUriStr();
}

void Uri::setFileExt(const std::string &fileExt) {
	_fileExt = su.trim(fileExt);
	updateUriStr();
}

void Uri::setFileNameAndExt(const std::string &fileNameAndExt) {
	std::vector<std::string> toks = su.tokenize(fileNameAndExt, '.');
	if (toks.empty())
		_fileName = su.trim(fileNameAndExt);
	else {
		_fileExt = su.trim("." + toks.at(toks.size() - 1));
		_fileName = "";
		for (size_t i = 0; i < toks.size() - 1; i++) {
			_fileName += su.trim(toks.at(i));
		}
	}
	updateUriStr();
}

void Uri::setQuery(const std::string &query) {
	_query = su.trim(query);
	updateUriStr();
}

void Uri::setPathInfo(const std::string &pathInfo) {
	_pathInfo = su.trim(pathInfo);
	updateUriStr();
}

void Uri::dump() const {
	Harl().debug("Uri: " + getUri());
	Harl().debug("Path: " + getPath());
	Harl().debug("Pathinfo: " + getPathInfo());
	Harl().debug("Query: " + getQuery());
	Harl().debug("File extension: " + getFileExtension());
	Harl().debug("File name: " + getFileName());
}

bool Uri::isDirectory() {
	return bDirectory;
}

Uri::Uri(Uri const &other) : bDirectory(other.bDirectory) {
	su = other.su;
	_uri = other._uri;
	_path = other._path;
	_fileName = other._fileName;
	_fileExt = other._fileExt;
	_pathInfo = other._pathInfo;
	_query = other._query;
	bDirectory = other.bDirectory;
	_uri = other._uri;
	if (this != &other)
		*this = other;
}

Uri& Uri::operator=(Uri const &other) {
	su = other.su;
	_uri = other._uri;
	_path = other._path;
	_fileName = other._fileName;
	_fileExt = other._fileExt;
	_pathInfo = other._pathInfo;
	_query = other._query;
	bDirectory = other.bDirectory;
	_uri = other._uri;

	return *this;
}

void Uri::updateUriStr() {
	std::string slash = "";
	if ((!_path.empty()) && _path.at(_path.size() - 1) != '/')
		slash = "/";
	Uri uriTmp(_path + slash + _fileName + _fileExt + _pathInfo + _query);
	bDirectory = uriTmp.bDirectory;
	_uri = uriTmp._uri;
}
