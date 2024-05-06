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

Processor* ProcessorFactory::build(std::string procName, Config *config)
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
	Processor *ret = NULL;

	if (procName == "PERL_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_PERL(typeContentModifier);
	else if (procName == "SH_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_SH(typeContentModifier);
	else if (procName == "STATIC_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_STATIC(typeContentModifier);
	else if (procName == "MIMETYPE_FILTER")
		ret = new REQUEST_HANDLER_IMPL_CLASS_MIMETYPE(typeHeaderModifier);
	else if (procName == "COMMON_FILTER")
		ret = new REQUEST_HANDLER_IMPL_CLASS_COMMON(typeHeaderModifier);
	else if (procName == "POST_FILTER")
		ret = new REQUEST_HANDLER_IMPL_CLASS_POST(typeHeaderModifier);
	else if (procName == "ERROR_FILTER")
		ret = new REQUEST_HANDLER_IMPL_CLASS_ERROR(typeHeaderModifier);
	else if (procName == "DELETE_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_DELETE(typeHeaderModifier);
	else if (procName == "REDIRECT_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_REDIRECT(typeContentModifier);
	else if (procName == "DOWNLOAD_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_DOWNLOAD(typeHeaderModifier);
	else
	{
		ret = new ProcessorImplCgiBinGeneric(typeContentModifier);

//		std::string cgiExePath = config->getParamStr(procName, "")
//		return cgiExePath;
	}
	ret->setConfig(config);
	return ret;
}

ProcessorFactory::ProcessorFactory() : processorLocator()
{
}

ProcessorFactory::~ProcessorFactory()
{
}
