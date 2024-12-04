#include "FileUtil.h"

#include <iostream>
#include <ostream>
#include <ctime>

FileUtil::FileUtil() {
}

FileUtil::~FileUtil() {
}

std::string FileUtil::getLastModification(std::string fpath, std::string strFormat /*EX: "%a, %d %b %Y %H:%M:%S %Z*/) {
	struct stat sb;
	if (stat(fpath.c_str(), &sb) == -1) {
		return "ERROR stat";
	}
	char buf[1000];
	time_t mtime = sb.st_mtime;
	struct tm *tmm = localtime(&mtime);
	strftime(buf, sizeof buf, strFormat.c_str(), tmm);
	std::string ret = std::string(buf);

	return ret;
}

int FileUtil::readFile(std::string path, char *emptyBodyBin[]) {
	std::ifstream is(path.c_str(), std::ios::binary);
	if (is.fail())
		return -1;

	// get length of file:
	is.seekg(0, is.end);
	int length = is.tellg();
//	length--;
	is.seekg(0, is.beg);

	*emptyBodyBin = new char[length]();
	memset(*emptyBodyBin, 0, length);

	is.read(*emptyBodyBin, length);
	is.close();

	return length;
}

std::string FileUtil::readTextFile(std::string path) {
	std::ifstream inputFile(path.c_str());
	if (!inputFile.is_open()) {
		return "";
	}

	std::string line;
	std::string content = "";
	while (getline(inputFile, line)) {
		content += line + "\r\n";
	}
	inputFile.close();

	return content;
}

int getdir(std::string dir, std::vector<std::string> &files) {
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error(" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(std::string(dirp->d_name));
	}
	closedir(dp);
	return 0;
}
std::vector<std::string> FileUtil::listDir(std::string path) {
	std::vector<std::string> files = std::vector<std::string>();
	getdir(path, files);

	return files;
}

bool FileUtil::fileExists(std::string path) {
	struct stat sb;
	bool ret = stat(path.c_str(), &sb) == 0;
	return ret;
}

bool FileUtil::writeFile(std::string path, char *data, size_t dataSize) {
	std::ofstream fileStream(path.c_str(), std::ios::binary);
	if (!fileStream) {
		std::cerr << "Failed to open file for writing: " << path << std::endl;
		return false;
	}
	fileStream.write(data, dataSize);
	fileStream.close();
	return true;
}

bool FileUtil::isDirectory(std::string path) {
	struct stat statbuf;
	if (stat(path.c_str(), &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}

std::string formatPath(const std::string &path) {
	std::string formattedPath;
	std::vector<std::string> parts;
	std::stringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '/')) {
		if (part == "..") {
			if (!parts.empty()) {
				parts.pop_back();
			}
		} else if (part != "." && !part.empty()) {
			parts.push_back(part);
		}
	}

	for (size_t i = 0; i < parts.size(); ++i) {
		const std::string &part = parts[i];
		formattedPath += "/" + part;
	}

	return formattedPath;

}

std::string FileUtil::realPathFile(std::string relativePath) {
	std::string ret;
	Harl harl;

	std::string rootPath = Config::ROOT_PATH;
	ret = rootPath + formatPath(relativePath);

	return ret;
}

