#include "Processor.h"

Processor::Processor(ProcessorTypeEnum type)
{
	this->type = type;
}

Processor::Processor() : type(CONTENT_MODIFIER)
{
}

Processor::~Processor()
{
}
