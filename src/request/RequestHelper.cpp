#include "RequestHelper.h"

RequestHelper::RequestHelper(char *const rawRequest, int const rawRequestTotalLen) {
	rawRequestBuffer = NULL;
	rawRequestBuffer_Length = 0;
	requestBodyLen = 0;
	bodyContent_Length = 0;

	bFragmentedMode = true;
	bJustInitialized = false;
	size_t rawRequestTotalLen_Local = rawRequestTotalLen;

	StringUtil su = StringUtil();

	RequestHeader *reqHeaderTemp = RequestHeaderFactory().build(rawRequest, rawRequestTotalLen);

	if (reqHeaderTemp) {

		int bodyLengthFromHeader = su.intFromStr(reqHeaderTemp->getFieldValue("Content-Length"));
		bodyContent_Length = bodyLengthFromHeader;

		harl.trace("\n\n==================\nRequestHelper::RequestHelper:\t pour uri = %s ", reqHeaderTemp->getUri().getUri().c_str());
		harl.trace("RequestHelper::RequestHelper:\t bodyContent_Length=%i", bodyContent_Length);
		RequestBody *reqBodyTmp = RequestBodyFactory().build(rawRequest, rawRequestTotalLen_Local, bodyContent_Length, reqHeaderTemp);
		harl.trace("RequestHelper::RequestHelper:\t *** Réception du premier fragment de taille %i", rawRequestTotalLen_Local);
		harl.trace("RequestHelper::process:\t Aggrégation de rawRequest, size=%i", rawRequestTotalLen_Local);

		rawRequestBuffer = new char[rawRequestTotalLen_Local]();
		memcpy(rawRequestBuffer, rawRequest, rawRequestTotalLen_Local);
		rawRequestBuffer_Length += rawRequestTotalLen_Local;

		rawRequestTotalLen_Local = 0;
		requestBodyLen = reqBodyTmp->getLen();
		harl.trace("RequestHelper::RequestHelper:\t taille du BODY actuel : %i", requestBodyLen);

		delete reqHeaderTemp;
		reqHeaderTemp = NULL;
		delete reqBodyTmp;
		reqBodyTmp = NULL;

		bFragmentedMode = (rawRequestTotalLen + requestBodyLen) < bodyContent_Length;
	}

	bJustInitialized = true;
	harl.trace("RequestHelper::RequestHelper:\t bFragmentedMode= %b", bFragmentedMode);
	harl.trace("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestTotalLen, requestBodyLen, bodyContent_Length);

}

char* RequestHelper::process(char *rawRequest, int rawRequestLen) {
	FileUtil fu = FileUtil();
	harl.trace("\n\n==================\nRequestHelper::process:\t rawRequestLen=%i", rawRequestLen);

	harl.trace("RequestHelper::process:\t bFragmentedMode=%b", bFragmentedMode);
	if (bJustInitialized) {
		bJustInitialized = false;
		if (bFragmentedMode) {
			harl.trace("RequestHelper::process:\t bJustInitialized=%b", bJustInitialized);

			harl.trace("RequestHelper::process:\t BYE ! ");
			return NULL;
		} else {
		}
	}

	if (!bJustInitialized && bFragmentedMode) {
		harl.trace("RequestHelper::process:\t Aggrégation de rawRequest, size=%i", rawRequestLen);
		requestBodyLen += rawRequestLen;
		char *rawRequestBufferTmp = new char[rawRequestBuffer_Length + rawRequestLen]();
		memcpy(rawRequestBufferTmp, rawRequestBuffer, rawRequestBuffer_Length);
		memcpy(rawRequestBufferTmp + rawRequestBuffer_Length, rawRequest, rawRequestLen);

		if (HARL_LEVEL >= 4) {
			std::string fname = "DBG/rawRequestBufferTmp.txt";
			fu.writeFile(fname.c_str(), rawRequestBufferTmp, rawRequestBuffer_Length + rawRequestLen);
		}

		delete rawRequestBuffer;
		rawRequestBuffer_Length += rawRequestLen;
		rawRequestBuffer = rawRequestBufferTmp;

		harl.trace("RequestHelper::process:\t requestBodyLen += %i = %i", rawRequestLen, requestBodyLen);
	}
	rawRequestLen = 0;

	harl.debug("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestLen, requestBodyLen, bodyContent_Length);
	bFragmentedMode = (rawRequestLen + requestBodyLen) < bodyContent_Length;
	harl.debug("RequestHelper::process:\t bFragmentedMode=%b", bFragmentedMode);

	if (!bFragmentedMode) {
		harl.debug("RequestHelper::process:\t *** Fin de la fragmentation ***");
		return (rawRequestBuffer);
	} else {
		harl.debug("RequestHelper::process:\t *** Fragmentation toujours en cours***");
		bFragmentedMode = (rawRequestLen + requestBodyLen) < bodyContent_Length;
		harl.debug("RequestHelper::process:\t Pour la prochaine itération : bFragmentedMode=%b", bFragmentedMode);
		harl.debug("RequestHelper::process:\t rawRequestLen + requestBodyLen < bodyContent_Length : %i + %i < %i", rawRequestLen, requestBodyLen, bodyContent_Length);
		return NULL;
	}

}

RequestHelper::RequestHelper() : rawRequestBuffer(NULL), rawRequestBuffer_Length(0), requestBodyLen(0), bodyContent_Length(0), bFragmentedMode(false), bJustInitialized(false)

{
}

RequestHelper::~RequestHelper() {
	delete[] rawRequestBuffer;
}

int RequestHelper::getRawRequestBufferLength() const {
	return rawRequestBuffer_Length;
}

void RequestHelper::setRawRequestBufferLength(int rawRequestBufferLength) {
	rawRequestBuffer_Length = rawRequestBufferLength;
}

