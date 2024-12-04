#pragma once
#include <string>
#include "MimeType.h"
#include "../util/StringUtil.h"
#include "../Harl.h"
#include "MimeTypeList.h"

class MimeTypeHelper {
private:

public:
	MimeTypeHelper();
	~MimeTypeHelper();
	std::string findMimeType(std::string extension);
};
