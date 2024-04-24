#include "ProcessorFactory.h"
#include "../Harl.h"

std::vector<ProcessorAndLocationToProcessor*>* ProcessorFactory::build(Request *request)
{
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
	ProcessorTypeEnum typeContentModifier = CONTENT_MODIFIER;
	ProcessorTypeEnum typeHeaderModifier = HEADER_MODIFIER;
	//	TODO new
//	if (procName == "PHP_PROCESSOR")
//	{
//		Processor *proc = new REQUEST_HANDLER_IMPL_CLASS_PHP(typeContentModifier);
//		return proc;
//	}
	Harl().debug("ProcessorFactory::build", "procName: " + procName);

	if (procName == "PERL_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_PERL(typeContentModifier);
	else if (procName == "SH_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_SH(typeContentModifier);
	else if (procName == "STATIC_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_STATIC(typeContentModifier);
	else if (procName == "MIMETYPE_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_MIMETYPE(typeHeaderModifier);
	else if (procName == "COMMON_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_COMMON(typeHeaderModifier);
	else if (procName == "POST_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_POST(typeHeaderModifier);
	else if (procName == "ERROR_FILTER")
		return new REQUEST_HANDLER_IMPL_CLASS_ERROR(typeHeaderModifier);
	else if (procName == "DELETE_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_DELETE(typeHeaderModifier);
	else if (procName == "REDIRECT_PROCESSOR")
		return new REQUEST_HANDLER_IMPL_CLASS_REDIRECT(typeContentModifier);
	//	TODO doit être configurable
//	return new ProcessorImplDirectFs(typeContentModifier);
	return NULL;
}

ProcessorFactory::ProcessorFactory() : processorLocator()
{
}

ProcessorFactory::~ProcessorFactory()
{
}
