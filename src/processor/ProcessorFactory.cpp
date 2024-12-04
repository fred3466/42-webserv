#include "ProcessorFactory.h"
#include "../Harl.h"

std::vector<ProcessorAndLocationToProcessor*>* ProcessorFactory::build(Request *request) {
	std::vector<ProcessorAndLocationToProcessor*> *procs = processorLocator->listOrderedProcessorForUrlAndExt(request);
	return procs;
}

ProcessorFactory::ProcessorFactory(ProcessorLocator *pl) {
	processorLocator = pl;
}

Processor* ProcessorFactory::build(std::string procName, Config *config) {
	ProcessorTypeEnum typeContentModifier = CONTENT_MODIFIER;
	ProcessorTypeEnum typeHeaderModifier = HEADER_MODIFIER;
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
	else if (procName == "DELETE_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_DELETE(typeHeaderModifier);
	else if (procName == "REDIRECT_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_REDIRECT(typeHeaderModifier);
	else if (procName == "DOWNLOAD_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_DOWNLOAD(typeHeaderModifier);
	else if (procName == "CGI_GENERIC_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_GENERIC_CGI(typeContentModifier);
	else if (procName == "COOKIE_PROCESSOR")
		ret = new REQUEST_HANDLER_IMPL_CLASS_COOKIE(typeHeaderModifier);
	else {
		ret = new ProcessorImplCgiBinGeneric(typeContentModifier);
	}
	ret->setConfig(config);
	return ret;
}

ProcessorFactory::ProcessorFactory() : processorLocator() {
}

ProcessorFactory::~ProcessorFactory() {
}
