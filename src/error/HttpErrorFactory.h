#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "HttpError.h"

class HttpErrorFactory {
private:

public:
	void loadErrorList();
	static std::vector<HttpError*> errorList;
	HttpErrorFactory();
	~HttpErrorFactory();
	HttpError* build(int errorCode);
	void clearList();
};

