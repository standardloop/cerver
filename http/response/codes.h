#ifndef CODES_H
#define CODES_H

#include <stdbool.h>

enum HTTPCode
{
    // 1XX
    HTTPContinue = 100,
    // FIXME: 102?
    HTTPSwitchingProtocols = 101,
    HTTPProcessing = 102,
    HTTPEarlyHints = 103,

    // 2XX
    HTTPOK = 200,
    HTTPCreated = 201,
    HTTPAccepted = 202,
    HTTPNonAuthoritativeInformation = 203,
    HTTPNoContent = 204,
    HTTPResetContent = 205,
    HTTPPartialContent = 206,
    HTTPMultiStatus = 207,
    HTTPAlreadyReported = 208,
    HTTPIMUsed = 226,

    // 3XX
    HTTPMultipleChoices = 300,
    HTTPMovedPermanently = 301,
    HTTPFound = 302,
    HTTPSeeOther = 303,
    HTTPNotModified = 304,
    HTTPUseProxy = 305, // DEPRECATED
    HTTPunused = 306,   // DEPRECATED
    HTTPTemporaryRedirect = 307,
    HTTPPermanentRedirect = 308,

    // 4XX
    HTTPBadRequest = 400,
    HTTPUnauthorized = 401,
    HTTPPaymentRequired = 402,
    HTTPForbidden = 403,
    HTTPNotFound = 404,
    HTTPMethodNotAllowed = 405,
    HTTPNotAcceptable = 406,
    HTTPProxyAuthenticationRequired = 407,
    HTTPRequestTimeout = 408,
    HTTPConflict = 409,
    HTTPGone = 410,
    HTTPLengthRequired = 411,
    HTTPPreconditionFailed = 412,
    HTTPContentTooLarge = 413,
    HTTPPayloadTooLarge = 413,
    HTTPURITooLong = 414,
    HTTPUnsupportedMediaType = 415,
    HTTPRangeNotSatisfiable = 416,
    HTTPExpectationFailed = 417,
    HTTPImATeapot = 418,
    HTTPMisdirectedRequest = 421,
    HTTPUnprocessableContent = 422,
    HTTPUnprocessableEntity = 422,
    HTTPLocked = 423,
    HTTPFailedDependency = 424,
    HTTPTooEarly = 425,
    HTTPUpgradeRequired = 426,
    HTTPPreconditionRequired = 428,
    HTTPTooManyRequests = 429,
    HTTPRequestHeaderFieldsTooLarge = 431,
    HTTPUnavailableForLegalReasons = 451,

    // 5XX
    HTTPInternalServerError = 500,
    HTTPNotImplemented = 501,
    HTTPBadGateway = 502,
    HTTPServiceUnavailable = 503,
    HTTPGatewayTimeout = 504,
    HTTPHTTPVersionNotSupported = 505,
    HTTPVariantAlsoNegotiates = 506,
    HTTPInsufficientStorage = 507,
    HTTPLoopDetected = 508,
    HTTPNotExtended = 510,
    HTTPNetworkAuthenticationRequired = 511,
};

char *HTTPClass(enum HTTPCode);
char *HTTPStatusCodeToString(enum HTTPCode, bool);

#endif
