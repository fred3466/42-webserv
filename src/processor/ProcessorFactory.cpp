#include "ProcessorFactory.h"

std::vector<ProcessorAndLocationToProcessor*>* ProcessorFactory::build(Request *request)
{
	Processor *proc;
//	std::string ext = request->getFileExtension();
//	std::string uri = request->getUri();

	std::vector<ProcessorAndLocationToProcessor*> *procs = processorLocator->listOrderedProcessorForUrlAndExt(request);
//	procs->insert(procs->begin(), new ProcessorImplDirectFs());

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
	ProcessorTypeEnum typeContentModifier = CONTENT_MODIFIER;
	ProcessorTypeEnum typeHeaderModifier = HEADER_MODIFIER;
	if (procName == "PHP_PROCESSOR")
	{
		Processor *proc = new REQUEST_HANDLER_IMPL_CLASS_PHP(typeContentModifier);
		return proc;
	}
	else if (procName == "STATIC_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_STATIC(typeContentModifier);
	else if (procName == "MIMETYPE_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_MIMETYPE(typeHeaderModifier);
	else if (procName == "COMMON_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_COMMON(typeHeaderModifier);
//	TODO doit être configurable
	return new ProcessorImplDirectFs(typeContentModifier);
}

ProcessorFactory::ProcessorFactory() : processorLocator()
{
}

ProcessorFactory::~ProcessorFactory()
{
}

