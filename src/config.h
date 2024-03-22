#define BUF_SIZE 1024
// trace = 4 "debug"=3, "info"=2, "warning"=1, "error"=0
#define HARL_LEVEL 3
#define RESPONSE_IMPL_INC "../ResponseHttp.h"
#define RESPONSE_IMPL_CLASS ResponseHttp
#define RESPONSE_HEADER_IMPL_INC "../ResponseHttpHeader.h"
#define RESPONSE_HEADER_IMPL_CLASS ResponseHttpHeader
#define REQUEST_IMPL_INC "../RequestHttp.h"
#define REQUEST_IMPL_CLASS RequestHttp
#define REQUEST_HEADER_IMPL_INC "../RequestHttpHeader.h"
#define REQUEST_HEADER_IMPL_CLASS RequestHttpHeader
#define REQUEST_HEADER_IMPL_INC_PHP "ProcessorImplCgiBinPhp.h"
#define REQUEST_HANDLER_IMPL_CLASS_PHP ProcessorImplCgiBinPhp
#define REQUEST_HEADER_IMPL_INC_STATIC "ProcessorImplDirectFs.h"
#define REQUEST_HANDLER_IMPL_CLASS_STATIC ProcessorImplDirectFs
#define REQUEST_HEADER_IMPL_INC_MIMETYPE "FiltreResponseMimeType.h"
#define REQUEST_HANDLER_IMPL_CLASS_MIMETYPE FiltreResponseMimeType
#define REQUEST_HEADER_IMPL_INC_COMMON "FiltreResponseCommon.h"
#define REQUEST_HANDLER_IMPL_CLASS_COMMON FiltreResponseCommon

