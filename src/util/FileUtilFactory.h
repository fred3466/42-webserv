#pragma once
#include <string>

#include "FileUtil.h"

class FileUtilFactory
{
public:
	FileUtil* build();
};

