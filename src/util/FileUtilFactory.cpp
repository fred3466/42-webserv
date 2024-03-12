#include "FileUtilFactory.h"

FileUtil* FileUtilFactory::build()
{
//	if(request.getUri()== ...
	return new FileUtil();
}
