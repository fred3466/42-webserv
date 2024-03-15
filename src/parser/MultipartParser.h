#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

class MultipartParser
{
private:
    std::string boundary;
    void handlePart(const std::string &part, const std::string &targetDir);
    void saveFile(const std::string &path, const std::string &content);

public:
    MultipartParser(const std::string &boundary);
    void parseMultipartData(const std::string &body, const std::string &targetDir);
};