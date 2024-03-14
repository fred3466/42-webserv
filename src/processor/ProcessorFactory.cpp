#include "ProcessorFactory.h"

std::vector<Processor*>* ProcessorFactory::build(Request *request)
{
//	TODO new à gogo!
	Processor *proc;
	std::string ext = request->getFileExtension();
	std::string uri = request->getUri();

	std::vector<Processor*> *procs = processorLocator->listOrderedProcessorForUrlAndExt(uri, ext);
	procs->insert(procs->begin(), new ProcessorImplDirectFs());
//	StringUtil stringUtil;
//	std::string uri = request->getUri();
//	std::string fileExt = uri.substr(uri.rfind(".", std::string::npos));
//	if (stringUtil.strUpper(fileExt) == ".PHP")
//	{
//		// TODO insérer ici l'implémentation pour CGI d'Anastasia
//		// return new ProcessorImplDirectFs();
//	}
//
//	processorLocator->listOrderedProcessorForUrlAndExt(urlPath, ext)
	return procs;
}

ProcessorFactory::ProcessorFactory(ProcessorLocator *pl)
{
	processorLocator = pl;
}

Processor* ProcessorFactory::build(std::string procName)
{
//	TODO new
	if (procName == "PHP_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_PHP();
//	TODO doit être configurable
	return new ProcessorImplDirectFs();
}

ProcessorFactory::ProcessorFactory() : processorLocator()
{
}

ProcessorFactory::~ProcessorFactory()
{
}

