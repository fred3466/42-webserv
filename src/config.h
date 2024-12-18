#define BUF_SIZE 1024
//	trace2=5 trace = 4 "debug"=3, "info"=2, "warning"=1, "error"=0 "exceptionnal"=-1
#define HARL_LEVEL 3

#define RFC_1123_DATE_FORMAT "%a, %d %b %Y %T %Z"
#define POLLFD_LIMIT 500
#define CGI_EXEC_TIMEOUT 2 * 1000 * 1000

#define RESPONSE_IMPL_INC "../ResponseHttp.h"
#define RESPONSE_IMPL_CLASS ResponseHttp
#define RESPONSE_HEADER_IMPL_INC "../ResponseHttpHeader.h"
#define RESPONSE_HEADER_IMPL_CLASS ResponseHttpHeader
#define REQUEST_IMPL_INC "../RequestHttp.h"
#define REQUEST_IMPL_CLASS RequestHttp
#define REQUEST_HEADER_IMPL_INC "../RequestHttpHeader.h"
#define REQUEST_HEADER_IMPL_CLASS RequestHttpHeader
#define REQUEST_BODY_IMPL_INC "../RequestHttpBody.h"
#define REQUEST_BODY_IMPL_CLASS RequestHttpBody

#define REQUEST_HEADER_IMPL_INC_STATIC "impl/ProcessorImplDirectFs.h"
#define REQUEST_HANDLER_IMPL_CLASS_STATIC ProcessorImplDirectFs
#define REQUEST_HEADER_IMPL_INC_MIMETYPE "impl/FiltreResponseMimeType.h"
#define REQUEST_HANDLER_IMPL_CLASS_MIMETYPE FiltreResponseMimeType
#define REQUEST_HEADER_IMPL_INC_COMMON "impl/FiltreResponseCommon.h"
#define REQUEST_HANDLER_IMPL_CLASS_COMMON FiltreResponseCommon
#define REQUEST_HEADER_IMPL_INC_POST "impl/FiltreRequestPost.h"
#define REQUEST_HANDLER_IMPL_CLASS_POST FiltreRequestPost
#define REQUEST_HEADER_IMPL_INC_SH "impl/ProcessorImplCgiBinSh.h"
#define REQUEST_HANDLER_IMPL_CLASS_SH ProcessorImplCgiBinSh
#define REQUEST_HANDLER_IMPL_INC_PERL "impl/ProcessorImplCgiBinPerl.h"
#define REQUEST_HANDLER_IMPL_CLASS_PERL ProcessorImplCgiBinPerl
#define REQUEST_HEADER_IMPL_INC_DELETE "impl/ProcessorImplDelete.h"
#define REQUEST_HANDLER_IMPL_CLASS_DELETE ProcessorImplDelete
#define REQUEST_HANDLER__IMPL_INC_REDIRECT "impl/FiltreResponseRedirect.h"
#define REQUEST_HANDLER_IMPL_CLASS_REDIRECT FiltreResponseRedirect
#define REQUEST_HANDLER__IMPL_INC_DOWNLOAD "impl/FiltreDownload.h"
#define REQUEST_HANDLER_IMPL_CLASS_DOWNLOAD FiltreDownload
#define REQUEST_HANDLER__IMPL_INC_GENERIC_CGI "impl/ProcessorImplCgiBinGeneric.h"
#define REQUEST_HANDLER_IMPL_CLASS_GENERIC_CGI ProcessorImplCgiBinGeneric
#define REQUEST_HANDLER__IMPL_INC_COOKIE "impl/FiltreCookie.h"
#define REQUEST_HANDLER_IMPL_CLASS_COOKIE FiltreCookie

#define CGI_HANDLER_INC_PERL "impl/CGIHandlerPerl.h"
#define CGI_HANDLER_CLASS_PERL CGIHandlerPerl

#define CGI_HANDLER_INC_SH "impl/CGIHandlerSh.h"
#define CGI_HANDLER_CLASS_SH CGIHandlerSh

#define CGI_HANDLER_INC_GENERIC "impl/CGIHandlerGeneric.h"
#define CGI_HANDLER_CLASS_GENERIC CGIHandlerGeneric

