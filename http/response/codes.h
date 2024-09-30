#ifndef CODES_H
#define CODES_H

#include <stdbool.h>

enum HttpCode
{
    // 1XX
    HttpContinue = 100,
    // FIXME: 102?
    HttpSwitchingProtocols = 101,
    HttpProcessing = 102,
    HttpEarlyHints = 103,

    // 2XX
    HttpOK = 200,
    HttpCreated = 201,
    HttpAccepted = 202,
    HttpNonAuthoritativeInformation = 203,
    HttpNoContent = 204,
    HttpResetContent = 205,
    HttpPartialContent = 206,
    HttpMultiStatus = 207,
    HttpAlreadyReported = 208,
    HttpIMUsed = 226,

    // 3XX
    HttpMultipleChoices = 300,
    HttpMovedPermanently = 301,
    HttpFound = 302,
    HttpSeeOther = 303,
    HttpNotModified = 304,
    HttpUseProxy = 305, // DEPRECATED
    Httpunused = 306,   // DEPRECATED
    HttpTemporaryRedirect = 307,
    HttpPermanentRedirect = 308,

    // 4XX
    HttpBadRequest = 400,
    HttpUnauthorized = 401,
    HttpPaymentRequired = 402,
    HttpForbidden = 403,
    HttpNotFound = 404,
    HttpMethodNotAllowed = 405,
    HttpNotAcceptable = 406,
    HttpProxyAuthenticationRequired = 407,
    HttpRequestTimeout = 408,
    HttpConflict = 409,
    HttpGone = 410,
    HttpLengthRequired = 411,
    HttpPreconditionFailed = 412,
    HttpContentTooLarge = 413,
    HttpPayloadTooLarge = 413,
    HttpURITooLong = 414,
    HttpUnsupportedMediaType = 415,
    HttpRangeNotSatisfiable = 416,
    HttpExpectationFailed = 417,
    HttpImATeapot = 418,
    HttpMisdirectedRequest = 421,
    HttpUnprocessableContent = 422,
    HttpUnprocessableEntity = 422,
    HttpLocked = 423,
    HttpFailedDependency = 424,
    HttpTooEarly = 425,
    HttpUpgradeRequired = 426,
    HttpPreconditionRequired = 428,
    HttpTooManyRequests = 429,
    HttpRequestHeaderFieldsTooLarge = 431,
    HttpUnavailableForLegalReasons = 451,

    // 5XX
    HttpInternalServerError = 500,
    HttpNotImplemented = 501,
    HttpBadGateway = 502,
    HttpServiceUnavailable = 503,
    HttpGatewayTimeout = 504,
    HttpHTTPVersionNotSupported = 505,
    HttpVariantAlsoNegotiates = 506,
    HttpInsufficientStorage = 507,
    HttpLoopDetected = 508,
    HttpNotExtended = 510,
    HttpNetworkAuthenticationRequired = 511,
};

char *HttpClass(enum HttpCode);
char *HttpStatusCodeToString(enum HttpCode, bool);

#endif
