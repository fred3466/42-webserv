#pragma once
#include "FileUtil.h"

#include <string>

class FileUtilFactory
{
public:
	FileUtil* build();
};

