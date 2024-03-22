#include "MimeTypeHelper.h"
#include <fstream>
#include <sstream>

MimeTypeHelper::MimeTypeHelper()
{
	std::ifstream file("conf/mime.types");
	std::string line;

	if (!file.is_open())
		return;

	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == ' ' || line[0] == '\t' || line[0] == '#')
			continue;

		std::istringstream lineStream(line);
		std::string mimeType;
		lineStream >> mimeType;

		std::string extension;
		while (lineStream >> extension)
		{
			addMapping(extension, mimeType);
		}
	}

	file.close();
}

MimeTypeHelper::~MimeTypeHelper()
{
}

void MimeTypeHelper::addMapping(const std::string &extension, const std::string &mimeType)
{
	mimeTypeList.push_back(MimeType(extension, mimeType));
}

std::string MimeTypeHelper::findMimeType(const std::string &extension) const
{
	for (std::vector<MimeType>::const_iterator it = mimeTypeList.begin(); it != mimeTypeList.end(); ++it)
	{
		if (it->getExtension() == extension)
		{
			return it->getMimeType();
		}
	}
	return "application/octet-stream"; // Default MIME type if not found
}

void MimeTypeHelper::reloadMappingsFromFile(const std::string &filePath)
{
	mimeTypeList.clear(); // Clear existing mappings

	std::ifstream file(filePath.c_str()); // Corrected line
	std::string line;

	if (!file.is_open())
	{
		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == ' ' || line[0] == '\t' || line[0] == '#')
		{
			continue;
		}

		std::istringstream lineStream(line);
		std::string mimeType;

		while (lineStream >> mimeType)
		{
			std::string extension;
			while (lineStream >> extension)
			{
				addMapping(extension, mimeType);
			}
		}
	}

	file.close();
}

size_t MimeTypeHelper::numberOfMappings() const
{
	return mimeTypeList.size(); // number of elements in mimeTypeList
}
