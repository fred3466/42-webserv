#include "MimeTypeHelper.h"
#include <fstream>
#include <sstream>

MimeTypeHelper::MimeTypeHelper() {

}

MimeTypeHelper::~MimeTypeHelper() {

}

std::string MimeTypeHelper::findMimeType(std::string extension) {
	std::string line;

	if (MimeTypeList::getSz() == 0) {
		std::ifstream file("conf/mime.types");

		StringUtil su;
		while (std::getline(file, line)) {
			if (line.empty() || line[0] == ' ' || line[0] == '\t' || line[0] == '#')
				continue;

			std::istringstream lineStream(line);
			std::string mimeType;
			lineStream >> mimeType;

			std::string extension;
			while (lineStream >> extension) {
				std::string extensionUpCase = su.strUpperCase(extension);
				MimeType *mtype = new MimeType(extensionUpCase, mimeType);
				MimeTypeList::push_back(mtype);
			}
		}

		file.close();
		Harl().warning("MimeTypeHelper::MimeTypeHelper : feed with %i mimetypes", MimeTypeList::getSz());
	}

	std::string ret = MimeTypeList::findMimeType(extension);
	return ret;
}

