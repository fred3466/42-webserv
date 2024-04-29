#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include "../Harl.h"

class MultipartParser
{
private:
	std::string boundary;
	Harl harl;
	void handlePart(std::string part, std::string targetDir);
	void saveFile(std::string path, std::string content);

public:
	MultipartParser(std::string boundary);
	void parseMultipartData(std::string body, std::string targetDir);
};
