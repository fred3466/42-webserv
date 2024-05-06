#include "RequestHelper.h"

//static std::string rawRequestBuffer = "";
//static size_t requestBodyLen = 0;
//static size_t bodyContent_Length = 0;
////static RequestHeader *reqHeaderTemp = NULL;
//static bool bFragmentedMode = false;
//static bool bJustInitialized = false;

RequestHelper::RequestHelper(char *const rawRequest, int const rawRequestLen)
{
	rawRequestBuffer = NULL;
	rawRequestBuffer_Length = 0;
	requestBodyLen = 0;
	bodyContent_Length = 0;
	bFragmentedMode = false;
	bJustInitialized = false;
	int rawRequestLen_Local = rawRequestLen;

	StringUtil su = StringUtil();

	RequestHeader *reqHeaderTemp = RequestHeaderFactory().build(rawRequest);
	bodyContent_Length = su.intFromStr(reqHeaderTemp->getFieldValue("Content-Length"));
//	harl.debug("\n\n==================\nRequestHelper::RequestHelper:\t pour uri = %s rawRequest = \n-------------------\n%s\n----------------------", reqHeaderTemp->getUri().getUri().c_str(),rawRequest.c_str());
	harl.debug("\n\n==================\nRequestHelper::RequestHelper:\t pour uri = %s ", reqHeaderTemp->getUri().getUri().c_str());
	harl.debug("RequestHelper::RequestHelper:\t bodyContent_Length=%i", bodyContent_Length);
//	if (bodyContent_Length != 0)
//	{
	RequestBody *reqBodyTmp = RequestBodyFactory().build(rawRequest, rawRequestLen_Local, reqHeaderTemp);
	harl.debug("RequestHelper::RequestHelper:\t *** Réception du premier fragment de taille %i", rawRequestLen_Local);
	harl.debug("RequestHelper::process:\t Aggrégation de rawRequest, size=%i", rawRequestLen_Local);

//	TODO new
	rawRequestBuffer = new char[rawRequestLen_Local]();
	memcpy(rawRequestBuffer, rawRequest, rawRequestLen_Local);
	rawRequestBuffer_Length += rawRequestLen_Local;

//	rawRequestBuffer.append(rawRequest);
	rawRequestLen_Local = 0;
	requestBodyLen = reqBodyTmp->getLen();
	harl.debug("RequestHelper::RequestHelper:\t taille du BODY actuel : %i", requestBodyLen);

	//			delete reqBodyTmp;
//	} else
//	{
//	}
	delete reqHeaderTemp;
	reqHeaderTemp = NULL;
	delete reqBodyTmp;
	reqBodyTmp = NULL;

	bFragmentedMode = (rawRequestLen + requestBodyLen) < bodyContent_Length;
	bJustInitialized = true;
	harl.debug("RequestHelper::RequestHelper:\t bFragmentedMode= %b", bFragmentedMode);
	harl.debug("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestLen, requestBodyLen, bodyContent_Length);

}

char* RequestHelper::process(char *rawRequest, int rawRequestLen)
{
	FileUtil fu = FileUtil();
	harl.debug("\n\n==================\nRequestHelper::process:\t rawRequestLen=%i", rawRequestLen);

	harl.debug("RequestHelper::process:\t bFragmentedMode=%b", bFragmentedMode);
	if (bJustInitialized)
	{
		bJustInitialized = false;
		if (bFragmentedMode)
		{
			harl.debug("RequestHelper::process:\t bJustInitialized=%b", bJustInitialized);

			harl.debug("RequestHelper::process:\t BYE ! ");
			return NULL;
		} else
		{
		}
	}

	if (!bJustInitialized && bFragmentedMode)
	{
		harl.debug("RequestHelper::process:\t Aggrégation de rawRequest, size=%i", rawRequestLen);
		requestBodyLen += rawRequestLen;
		char *rawRequestBufferTmp = new char[rawRequestBuffer_Length + rawRequestLen]();
		memcpy(rawRequestBufferTmp, rawRequestBuffer, rawRequestBuffer_Length);
		memcpy(rawRequestBufferTmp + rawRequestBuffer_Length, rawRequest, rawRequestLen);

		if (HARL_LEVEL >= 4)
		{
			std::string fname = "DBG/rawRequestBufferTmp.txt";
			fu.writeFile(fname.c_str(), rawRequestBufferTmp, rawRequestBuffer_Length + rawRequestLen);
		}

		delete rawRequestBuffer;
		rawRequestBuffer_Length += rawRequestLen;
		rawRequestBuffer = rawRequestBufferTmp;

		harl.debug("RequestHelper::process:\t requestBodyLen += %i = %i", rawRequestLen, requestBodyLen);
	}
	rawRequestLen = 0;

	harl.debug("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestLen, requestBodyLen, bodyContent_Length);
	bFragmentedMode = (rawRequestLen + requestBodyLen) < bodyContent_Length;
	harl.debug("RequestHelper::process:\t bFragmentedMode=%b", bFragmentedMode);

	if (!bFragmentedMode)
	{
		harl.debug("RequestHelper::process:\t *** Fin de la fragmentation ***");
		return (rawRequestBuffer);
	} else
	{
		harl.debug("RequestHelper::process:\t *** Fragmentation toujours en cours***");
		bFragmentedMode = (rawRequestLen + requestBodyLen) < bodyContent_Length;
		harl.debug("RequestHelper::process:\t Pour la prochaine itération : bFragmentedMode=%b", bFragmentedMode);
		harl.debug("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestLen, requestBodyLen, bodyContent_Length);
		return NULL;
	}

}

RequestHelper::RequestHelper() : rawRequestBuffer(NULL),
		rawRequestBuffer_Length(0),
		requestBodyLen(0),
		bodyContent_Length(0),
		bFragmentedMode(false),
		bJustInitialized(false)

{
}
RequestHelper::~RequestHelper()
{
	delete[] rawRequestBuffer;
}

int RequestHelper::getRawRequestBufferLength() const
{
	return rawRequestBuffer_Length;
}

void RequestHelper::setRawRequestBufferLength(int rawRequestBufferLength)
{
	rawRequestBuffer_Length = rawRequestBufferLength;
}

