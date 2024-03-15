#include "MultipartParser.h"

MultipartParser::MultipartParser(const std::string &boundary) : boundary(boundary) {}

void MultipartParser::parseMultipartData(const std::string &body, const std::string &targetDir)
{
    std::string delimiter = "--" + boundary + "\r\n";
    std::string endDelimiter = "--" + boundary + "--";

    size_t pos = 0;
    size_t nextPos = body.find(delimiter, pos);

    while (nextPos != std::string::npos)
    {
        pos = nextPos + delimiter.size();
        nextPos = body.find(delimiter, pos);
        if (nextPos == std::string::npos)
            break;

        std::string part = body.substr(pos, nextPos - pos);
        handlePart(part, targetDir);
    }
}

void MultipartParser::handlePart(const std::string &part, const std::string &targetDir)
{
    // Extract filename and file content
    std::string filenameHeader = "filename=\"";
    size_t filenamePos = part.find(filenameHeader);
    if (filenamePos != std::string::npos)
    {
        size_t start = filenamePos + filenameHeader.size();
        size_t end = part.find("\"", start);
        std::string filename = part.substr(start, end - start);

        // Assuming file content starts after "\r\n\r\n"
        size_t contentStart = part.find("\r\n\r\n", end) + 4; // Skipping "\r\n\r\n"
        std::string fileContent = part.substr(contentStart);

        // Remove trailing "\r\n" from file content
        if (!fileContent.empty())
        {
            fileContent.erase(fileContent.size() - 2, 2);
        }

        saveFile(targetDir + "/" + filename, fileContent);
    }
}

void MultipartParser::saveFile(const std::string &path, const std::string &content)
{
    std::ofstream file(path.c_str(), std::ios::binary);
    if (file.is_open())
    {
        file << content;
        file.close();
        std::cout << "Saved file: " << path << std::endl;
    }
    else
    {
        std::cerr << "Failed to save file: " << path << std::endl;
    }
}
