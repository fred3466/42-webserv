#include "ProcessorFactory.h"

Processor *ProcessorFactory::build(Request *request)
{
	StringUtil stringUtil;
	std::string uri = request->getUri();
	std::string fileExt = uri.substr(uri.rfind(".", std::string::npos));
	if (stringUtil.strUpper(fileExt) == ".PHP")
	{
		// TODO insérer ici l'implémentation pour CGI d'Anastasia
		// return new ProcessorImplDirectFs();
	}
	return new ProcessorImplDirectFs();
}
