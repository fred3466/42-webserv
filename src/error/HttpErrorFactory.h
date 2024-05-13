#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include "HttpError.h"

class HttpErrorFactory
{
private:
	std::vector<HttpError*> errorList;
	void loadErrorList();
	//	Config *config;

public:
	HttpErrorFactory();
	~HttpErrorFactory();
	//	void initialize(Config *config);
	HttpError* build(int errorCode);
};

