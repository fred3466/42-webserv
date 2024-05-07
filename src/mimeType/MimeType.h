#pragma once
#include <string>

class MimeType
{
private:
	std::string extension; // Key
	std::string mimeType;  // Value

public:
	MimeType();
	MimeType(std::string ext, std::string mType);
	~MimeType();
	MimeType(MimeType &o);
	MimeType& operator=(MimeType &o);

	std::string getExtension();
	std::string getMimeType();
	void setExtension(std::string ext);
	void setMimeType(std::string mType);
};
