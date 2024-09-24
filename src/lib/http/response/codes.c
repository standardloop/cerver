#include <stdlib.h>
#include "codes.h"

char *HttpStatusClass(enum HttpCode code)
{
    if (code >= 100 && code < 200)
    {
        return "1XX";
    }
    else if (code >= 200 && code < 300)
    {
        return "2XX";
    }
    else if (code >= 300 && code < 400)
    {
        return "3XX";
    }
    else if (code >= 400 && code < 500)
    {
        return "4XX";
    }
    else if (code >= 500 && code < 600)
    {
        return "5XX";
    }
    return NULL;
}

char *HttpStatusCodeToString(enum HttpCode code)
{
    switch (code)
    {
    // 1XX
    case HttpContinue:
        return "Continue";
    case HttpSwitchingProtocols:
        return "Switching Protocols";
    case HttpProcessing:
        return "Processing";
    case HttpEarlyHints:
        return "Early Hints";

    // 2XX
    case HttpOK:
        return "OK";
    case HttpCreated:
        return "Created";
    case HttpAccepted:
        return "Accepted";
    case HttpNonAuthoritativeInformation:
        return "Non-Authoritative Information";
    case HttpNoContent:
        return "No Content";
    case HttpResetContent:
        return "Reset Content";
    case HttpPartialContent:
        return "Partial Content";
    case HttpMultiStatus:
        return "Multi-Status";
    case HttpAlreadyReported:
        return "Already Reported";
    case HttpIMUsed:
        return "IM Used";

    // 3XX
    case HttpMultipleChoices:
        return "Multiple Choices";
    case HttpMovedPermanently:
        return "Moved Permanently";
    case HttpFound:
        return "Found";
    case HttpSeeOther:
        return "See Other";
    case HttpNotModified:
        return "Not Modified";
    case HttpTemporaryRedirect:
        return "Temporary Redirect";
    case HttpPermanentRedirect:
        return "Permanent Redirect";

    // 4XX
    case HttpBadRequest:
        return "Bad Request";
    case HttpUnauthorized:
        return "Unauthorized";
    case HttpPaymentRequired:
        return "Payment Required";
    case HttpForbidden:
        return "Forbidden";
    case HttpNotFound:
        return "Not Found";
    case HttpMethodNotAllowed:
        return "Method Not Allowed";
    case HttpNotAcceptable:
        return "Not Acceptable";
    case HttpProxyAuthenticationRequired:
        return "Proxy Authentication Required";
    case HttpRequestTimeout:
        return "Request Timeout";
    case HttpConflict:
        return "Conflict";
    case HttpGone:
        return "Gone";
    case HttpLengthRequired:
        return "Length Required";
    case HttpPreconditionFailed:
        return "Precondition Failed";
    case HttpContentTooLarge:
        return "Content Too Large";
    case HttpURITooLong:
        return "URI Too Long";
    case HttpUnsupportedMediaType:
        return "Unsupported Media Type";
    case HttpRangeNotSatisfiable:
        return "Range Not Satisfiable";
    case HttpExpectationFailed:
        return "Expectation Failed";
    case HttpImATeapot:
        return "I'm a teapot";
    case HttpMisdirectedRequest:
        return "Misdirected Request";
    case HttpUnprocessableContent:
        return "Unprocessable Content";
    case HttpLocked:
        return "Locked";
    case HttpFailedDependency:
        return "Failed Dependency";
    case HttpTooEarly:
        return "Too Early";
    case HttpUpgradeRequired:
        return "Upgrade Required";
    case HttpPreconditionRequired:
        return "Precondition Required";
    case HttpTooManyRequests:
        return "Too Many Requests";
    case HttpRequestHeaderFieldsTooLarge:
        return "Request Header Fields Too Large";
    case HttpUnavailableForLegalReasons:
        return "Unavailable For Legal Reasons";

    // 5XX
    case HttpInternalServerError:
        return "Internal Server Error";
    case HttpNotImplemented:
        return "Not Implemented";
    case HttpBadGateway:
        return "Bad Gateway";
    case HttpServiceUnavailable:
        return "Service Unavailable";
    case HttpGatewayTimeout:
        return "Gateway Timeout";
    case HttpHTTPVersionNotSupported:
        return "HTTP Version Not Supported";
    case HttpVariantAlsoNegotiates:
        return "Variant Also Negotiates";
    case HttpInsufficientStorage:
        return "Insufficient Storage";
    case HttpLoopDetected:
        return "Loop Detected";
    case HttpNotExtended:
        return "Not Extended";
    case HttpNetworkAuthenticationRequired:
        return "Network Authentication Required";

    default:
        return 0;
    }
}
