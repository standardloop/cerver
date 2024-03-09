#ifndef CODES_H
#define CODES_H
char *HttpStatus_Class(int code);
char *HttpStatus_Phrase(int code);
enum HttpCodes
{
    // 1XX
    HttpStatus_Continue = 100,
    HttpStatus_SwitchingProtocols = 101,
    HttpStatus_EarlyHints = 103,

    // 2XX
    HttpStatus_OK = 200,
    HttpStatus_Created = 201,
    HttpStatus_Accepted = 202,
    HttpStatus_NonAuthoritativeInformation = 203,
    HttpStatus_NoContent = 204,
    HttpStatus_ResetContent = 205,
    HttpStatus_PartialContent = 206,
    HttpStatus_MultiStatus = 207,
    HttpStatus_AlreadyReported = 208,
    HttpStatus_IMUsed = 226,

    // 3XX
    HttpStatus_MultipleChoices = 300,
    HttpStatus_MovedPermanently = 301,
    HttpStatus_Found = 302,
    HttpStatus_SeeOther = 303,
    HttpStatus_NotModified = 304,
    HttpStatus_UseProxy = 305,
    HttpStatus_TemporaryRedirect = 307,
    HttpStatus_PermanentRedirect = 308,

    // 4XX
    HttpStatus_BadRequest = 400,
    HttpStatus_Unauthorized = 401,
    HttpStatus_PaymentRequired = 402,
    HttpStatus_Forbidden = 403,
    HttpStatus_NotFound = 404,
    HttpStatus_MethodNotAllowed = 405,
    HttpStatus_NotAcceptable = 406,
    HttpStatus_ProxyAuthenticationRequired = 407,
    HttpStatus_RequestTimeout = 408,
    HttpStatus_Conflict = 409,
    HttpStatus_Gone = 410,
    HttpStatus_LengthRequired = 411,
    HttpStatus_PreconditionFailed = 412,
    HttpStatus_ContentTooLarge = 413,
    HttpStatus_PayloadTooLarge = 413,
    HttpStatus_URITooLong = 414,
    HttpStatus_UnsupportedMediaType = 415,
    HttpStatus_RangeNotSatisfiable = 416,
    HttpStatus_ExpectationFailed = 417,
    HttpStatus_ImATeapot = 418,
    HttpStatus_MisdirectedRequest = 421,
    HttpStatus_UnprocessableContent = 422,
    HttpStatus_UnprocessableEntity = 422,
    HttpStatus_Locked = 423,
    HttpStatus_FailedDependency = 424,
    HttpStatus_TooEarly = 425,
    HttpStatus_UpgradeRequired = 426,
    HttpStatus_PreconditionRequired = 428,
    HttpStatus_TooManyRequests = 429,
    HttpStatus_RequestHeaderFieldsTooLarge = 431,
    HttpStatus_UnavailableForLegalReasons = 451,

    // 5XX
    HttpStatus_InternalServerError = 500,
    HttpStatus_NotImplemented = 501,
    HttpStatus_BadGateway = 502,
    HttpStatus_ServiceUnavailable = 503,
    HttpStatus_GatewayTimeout = 504,
    HttpStatus_HTTPVersionNotSupported = 505,
    HttpStatus_VariantAlsoNegotiates = 506,
    HttpStatus_InsufficientStorage = 507,
    HttpStatus_LoopDetected = 508,
    HttpStatus_NotExtended = 510,
    HttpStatus_NetworkAuthenticationRequired = 511,

    HttpStatus_xxx_max = 1023
};
#endif
