#include "UriValidator.h"

std::string UriValidator::formatPath(const std::string &path)
{
	std::string formattedPath;
	std::vector<std::string> parts;
	std::stringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '/'))
	{
		if (part == "..")
		{
			if (!parts.empty())
			{
				parts.pop_back();
			}
		}
		else if (part != "." && !part.empty())
		{
			parts.push_back(part);
		}
	}

	for (size_t i = 0; i < parts.size(); ++i)
	{
		const std::string& part = parts[i];
		formattedPath += "/" + part;
	}

	return formattedPath;
	
}