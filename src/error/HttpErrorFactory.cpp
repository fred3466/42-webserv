#include "HttpErrorFactory.h"

std::vector<HttpError*> HttpErrorFactory::errorList = std::vector<HttpError*>();

void HttpErrorFactory::loadErrorList() {
	if (!errorList.empty())
		return;

	std::string fpath = "conf/errors.txt";
	std::ifstream file(fpath.c_str(), std::ios::in);
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			int code;
			std::string message;
			if (iss >> code && std::getline(iss >> std::ws, message)) {
				HttpError *error = new HttpError(code, message);
				errorList.push_back(error);
			}
		}
		file.close();
	}
}

HttpError* HttpErrorFactory::build(int errorCode) {
	if (errorList.empty())
		loadErrorList();

	for (std::vector<HttpError*>::iterator it = errorList.begin(); it != errorList.end(); ++it) {
		HttpError *httpError = *it;
		if (httpError->getCode() == errorCode) {
			return httpError;
		}
	}
	return new HttpError(errorCode, "Unknown Error");
}

HttpErrorFactory::HttpErrorFactory() {

}

void HttpErrorFactory::clearList() {
	for (std::vector<HttpError*>::iterator it = errorList.begin(); it != errorList.end(); ++it) {
		HttpError *httpError = *it;
		if (!httpError) {
			continue;
		}
		if (httpError)
			delete httpError;
	}
	errorList.erase(errorList.begin(), errorList.end());
	errorList.clear();
}

HttpErrorFactory::~HttpErrorFactory() {
	clearList();
}

