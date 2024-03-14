#include "MimeType.h"

MimeType::MimeType() : extension(""), mimeType("") {}

MimeType::MimeType(const std::string &ext, const std::string &mType)
    : extension(ext), mimeType(mType) {}

MimeType::~MimeType() {}

std::string MimeType::getExtension() const
{
    return extension;
}

std::string MimeType::getMimeType() const
{
    return mimeType;
}

void MimeType::setExtension(const std::string &ext)
{
    extension = ext;
}

void MimeType::setMimeType(const std::string &mType)
{
    mimeType = mType;
}
