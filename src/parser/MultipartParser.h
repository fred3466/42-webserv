#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include "../config.h"
#include "../Harl.h"
#include "../util/StringUtil.h"
#include "../util/FileUtil.h"

class MultipartParser {
private:
	std::string boundary;
	Harl harl;
	StringUtil su;
	void handlePart(char *part, int partLen, std::string targetDir);
	void saveFile(std::string path, char *content, int contentLen);

public:
	MultipartParser(std::string boundary);
	void parseMultipartData(char *body, int bodyLen, std::string targetDir);
};
