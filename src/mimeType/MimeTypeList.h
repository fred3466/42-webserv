#pragma once
#include "MimeType.h"
#include "../util/StringUtil.h"
#include "../Harl.h"

class MimeTypeList {
private:
	static MimeType **_arr;
	static int sz;

public:

	static void push_back(MimeType *mt);
	static std::string findMimeType(std::string extension);
	static int getSz();
	static void cleanUp();
};

