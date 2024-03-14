#pragma once
#include <string>

class MimeType
{
private:
    std::string extension; // Key
    std::string mimeType;  // Value

public:
    MimeType();
    MimeType(const std::string &ext, const std::string &mType);
    ~MimeType();

    std::string getExtension() const;
    std::string getMimeType() const;
    void setExtension(const std::string &ext);
    void setMimeType(const std::string &mType);
};
