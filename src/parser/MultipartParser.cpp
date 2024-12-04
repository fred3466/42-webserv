#include "MultipartParser.h"

MultipartParser::MultipartParser(std::string boundary) : boundary(boundary), harl() {
}

void MultipartParser::parseMultipartData(char *body, int bodyLen, std::string targetDir) {
	boundary = "--" + boundary;
	std::string delimiter = boundary + "";
	std::string endDelimiter = boundary + "--";

//	if (HARL_LEVEL >= 4) {
//		FileUtil fu = FileUtil();
//		std::string fname = "DBG/_body.txt";
//		fu.writeFile(fname.c_str(), body, bodyLen);
//	}

	int pos = 0;
	int nextPos = su.findStringInCString(body, bodyLen, delimiter, pos);

	while (nextPos != -1) {
		pos = nextPos + delimiter.size();
		nextPos = su.findStringInCString(body, bodyLen - pos, delimiter, pos);
		if (nextPos == -1)
			break;

		int partLen = nextPos - pos;
		char *part = new char[partLen];
		memcpy(part, body + pos, partLen);
		handlePart(part, partLen, targetDir);
	}
}

void MultipartParser::handlePart(char *part, int partLen, std::string targetDir) {
	std::string filenameHeader = "filename=\"";
	int filenamePos = su.findStringInCString(part, partLen, filenameHeader, 0);
	if (filenamePos != -1) {
		int start = filenamePos + filenameHeader.size();
		int maxLen = partLen - start;
		int end = su.findStringInCString(part, maxLen, "\"", start);
		char *filenameCString = new char[end - start + 1];
		filenameCString[end - start] = 0;
		memcpy(filenameCString, part + start, end - start);
		std::string filename = std::string(filenameCString, end - start);
		std::string absFname = targetDir.append("/").append(filename);

		delete[] filenameCString;

		// Assuming file content starts after "\r\n\r\n"
		int contentStart = su.findStringInCString(part, partLen - end, "\r\n\r\n", end) + 4; // Skipping "\r\n\r\n"

		int fileContentLen = partLen - contentStart - 2; // Remove trailing "\r\n" from file content
		char *fileContentCString = new char[fileContentLen];
		memcpy(fileContentCString, part + contentStart, fileContentLen);

		delete[] part;

		saveFile(absFname, fileContentCString, fileContentLen);

		delete[] fileContentCString;
	}
}

void MultipartParser::saveFile(std::string path, char *content, int contentLen) {
	FileUtil fu = FileUtil();
	bool success = fu.writeFile(path, content, contentLen);

	if (success) {
		harl.debug("MultipartParser::saveFile:\t XXXXXXXXX Saved file: %s", path.c_str());
	} else {
		harl.error("MultipartParser::saveFile:\t --------- Failed to save file: %s", path.c_str());
	}
}
