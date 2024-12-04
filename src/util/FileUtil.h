#pragma once

#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../Harl.h"
#include "../config/Config.h"

class FileUtil {
public:
	FileUtil();
	~FileUtil();

	int readFile(std::string path, char **emptyBodyBin);
	std::vector<std::string> listDir(std::string path);
	bool fileExists(std::string path);
	bool writeFile(std::string path, char *data, size_t dataSize);
	std::string getLastModification(std::string fpath, std::string strFormat);
	bool isDirectory(std::string path);
	std::string realPathFile(std::string path);
	std::string readTextFile(std::string path);
};
