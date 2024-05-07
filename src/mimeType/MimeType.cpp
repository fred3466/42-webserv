#include "MimeType.h"

MimeType::MimeType()
{
	extension = "";
	mimeType = "";

}

MimeType::MimeType(std::string ext, std::string mType)
{
	extension = ext;
	mimeType = mType;
}

MimeType::~MimeType()
{
}

std::string MimeType::getExtension()
{
	return extension;
}

std::string MimeType::getMimeType()
{
	return mimeType;
}

void MimeType::setExtension(std::string ext)
{
	extension = ext;
}

void MimeType::setMimeType(std::string mType)
{
	mimeType = mType;
}

MimeType::MimeType(MimeType &o)
{
	extension = o.extension;
	mimeType = o.mimeType;

	if (this != &o)
		*this = o;
}

MimeType& MimeType::operator=(MimeType &o)
{
	extension = o.extension;
	mimeType = o.mimeType;
	return *this;
}
