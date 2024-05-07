#pragma once
#include <vector>
#include <string>
#include "MimeType.h"

class MimeTypeHelper
{
private:
	std::vector<MimeType*> mimeTypeList;

public:
	MimeTypeHelper();
	~MimeTypeHelper();
	void addMapping(std::string extension, std::string mimeType);
	std::string findMimeType(std::string extension);
	void reloadMappingsFromFile(const std::string &filePath);
	size_t numberOfMappings() const;
};
