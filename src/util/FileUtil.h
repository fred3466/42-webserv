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

class FileUtil
{
public:
	FileUtil();
	~FileUtil();

	int readFile(std::string path, char **emptyBodyBin);
	std::vector<std::string> listDir(std::string path);
	bool fileExists(std::string path);
};

