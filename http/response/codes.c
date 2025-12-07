#include <stdlib.h>
#include <stdbool.h>

#include "codes.h"

char *HTTPStatusClass(enum HTTPCode code)
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
char *HTTPStatusCodeToString(enum HTTPCode code, bool message)
{
    switch (code)
    {
    // 1XX
    case HTTPContinue:
        return message ? "Continue" : "100";
    case HTTPSwitchingProtocols:
        return message ? "Switching Protocols" : "101";
    case HTTPProcessing:
        return message ? "Processing" : "102";
    case HTTPEarlyHints:
        return message ? "Early Hints" : "103";

    // 2XX
    case HTTPOK:
        return message ? "OK" : "200";
    case HTTPCreated:
        return message ? "Created" : "201";
    case HTTPAccepted:
        return message ? "Accepted" : "202";
    case HTTPNonAuthoritativeInformation:
        return message ? "Non-Authoritative Information" : "203";
    case HTTPNoContent:
        return message ? "No Content" : "204";
    case HTTPResetContent:
        return message ? "Reset Content" : "205";
    case HTTPPartialContent:
        return message ? "Partial Content" : "206";
    case HTTPMultiStatus:
        return message ? "Multi-Status" : "207";
    case HTTPAlreadyReported:
        return message ? "Already Reported" : "208";
    case HTTPIMUsed:
        return message ? "IM Used" : "226";

    // 3XX
    case HTTPMultipleChoices:
        return message ? "Multiple Choices" : "300";
    case HTTPMovedPermanently:
        return message ? "Moved Permanently" : "301";
    case HTTPFound:
        return message ? "Found" : "302";
    case HTTPSeeOther:
        return message ? "See Other" : "303";
    case HTTPNotModified:
        return message ? "Not Modified" : "304";
    case HTTPTemporaryRedirect:
        return message ? "Temporary Redirect" : "307";
    case HTTPPermanentRedirect:
        return message ? "Permanent Redirect" : "308";

    // 4XX
    case HTTPBadRequest:
        return message ? "Bad Request" : "400";
    case HTTPUnauthorized:
        return message ? "Unauthorized" : "401";
    case HTTPPaymentRequired:
        return message ? "Payment Required" : "402";
    case HTTPForbidden:
        return message ? "Forbidden" : "403";
    case HTTPNotFound:
        return message ? "Not Found" : "404";
    case HTTPMethodNotAllowed:
        return message ? "Method Not Allowed" : "405";
    case HTTPNotAcceptable:
        return message ? "Not Acceptable" : "406";
    case HTTPProxyAuthenticationRequired:
        return message ? "Proxy Authentication Required" : "407";
    case HTTPRequestTimeout:
        return message ? "Request Timeout" : "408";
    case HTTPConflict:
        return message ? "Conflict" : "409";
    case HTTPGone:
        return message ? "Gone" : "410";
    case HTTPLengthRequired:
        return message ? "Length Required" : "411";
    case HTTPPreconditionFailed:
        return message ? "Precondition Failed" : "412";
    case HTTPContentTooLarge:
        return message ? "Content Too Large" : "413";
    case HTTPURITooLong:
        return message ? "URI Too Long" : "414";
    case HTTPUnsupportedMediaType:
        return message ? "Unsupported Media Type" : "415";
    case HTTPRangeNotSatisfiable:
        return message ? "Range Not Satisfiable" : "416";
    case HTTPExpectationFailed:
        return message ? "Expectation Failed" : "417";
    case HTTPImATeapot:
        return message ? "I'm a teapot" : "418";
    case HTTPMisdirectedRequest:
        return message ? "Misdirected Request" : "421";
    case HTTPUnprocessableContent:
        return message ? "Unprocessable Content" : "422";
    case HTTPLocked:
        return message ? "Locked" : "423";
    case HTTPFailedDependency:
        return message ? "Failed Dependency" : "424";
    case HTTPTooEarly:
        return message ? "Too Early" : "425";
    case HTTPUpgradeRequired:
        return message ? "Upgrade Required" : "426";
    case HTTPPreconditionRequired:
        return message ? "Precondition Required" : "428";
    case HTTPTooManyRequests:
        return message ? "Too Many Requests" : "429";
    case HTTPRequestHeaderFieldsTooLarge:
        return message ? "Request Header Fields Too Large" : "431";
    case HTTPUnavailableForLegalReasons:
        return message ? "Unavailable For Legal Reasons" : "451";

    // 5XX
    case HTTPInternalServerError:
        return message ? "Internal Server Error" : "500";
    case HTTPNotImplemented:
        return message ? "Not Implemented" : "501";
    case HTTPBadGateway:
        return message ? "Bad Gateway" : "502";
    case HTTPServiceUnavailable:
        return message ? "Service Unavailable" : "503";
    case HTTPGatewayTimeout:
        return message ? "Gateway Timeout" : "504";
    case HTTPHTTPVersionNotSupported:
        return message ? "HTTP Version Not Supported" : "505";
    case HTTPVariantAlsoNegotiates:
        return message ? "Variant Also Negotiates" : "506";
    case HTTPInsufficientStorage:
        return message ? "Insufficient Storage" : "507";
    case HTTPLoopDetected:
        return message ? "Loop Detected" : "508";
    case HTTPNotExtended:
        return message ? "Not Extended" : "510";
    case HTTPNetworkAuthenticationRequired:
        return message ? "Network Authentication Required" : "511";

    default:
        return 0;
    }
}
