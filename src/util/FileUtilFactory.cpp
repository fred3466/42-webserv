#include "FileUtilFactory.h"

FileUtil* FileUtilFactory::build() {
	return new FileUtil();
}
