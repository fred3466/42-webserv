#pragma once
#include <vector>
#include <string>
#include "MimeType.h"

class MimeTypeHelper
{
private:
    std::vector<MimeType> mimeTypeList;

public:
    MimeTypeHelper();
    ~MimeTypeHelper();
    void addMapping(const std::string &extension, const std::string &mimeType);
    std::string findMimeType(const std::string &extension) const;
    void reloadMappingsFromFile(const std::string &filePath);
    size_t numberOfMappings() const;
};
