#include <stdlib.h>
#include <stdbool.h>

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
char *HttpStatusCodeToString(enum HttpCode code, bool message)
{
    switch (code)
    {
    // 1XX
    case HttpContinue:
        return message ? "Continue" : "100";
    case HttpSwitchingProtocols:
        return message ? "Switching Protocols" : "101";
    case HttpProcessing:
        return message ? "Processing" : "102";
    case HttpEarlyHints:
        return message ? "Early Hints" : "103";

    // 2XX
    case HttpOK:
        return message ? "OK" : "200";
    case HttpCreated:
        return message ? "Created" : "201";
    case HttpAccepted:
        return message ? "Accepted" : "202";
    case HttpNonAuthoritativeInformation:
        return message ? "Non-Authoritative Information" : "203";
    case HttpNoContent:
        return message ? "No Content" : "204";
    case HttpResetContent:
        return message ? "Reset Content" : "205";
    case HttpPartialContent:
        return message ? "Partial Content" : "206";
    case HttpMultiStatus:
        return message ? "Multi-Status" : "207";
    case HttpAlreadyReported:
        return message ? "Already Reported" : "208";
    case HttpIMUsed:
        return message ? "IM Used" : "226";

    // 3XX
    case HttpMultipleChoices:
        return message ? "Multiple Choices" : "300";
    case HttpMovedPermanently:
        return message ? "Moved Permanently" : "301";
    case HttpFound:
        return message ? "Found" : "302";
    case HttpSeeOther:
        return message ? "See Other" : "303";
    case HttpNotModified:
        return message ? "Not Modified" : "304";
    case HttpTemporaryRedirect:
        return message ? "Temporary Redirect" : "307";
    case HttpPermanentRedirect:
        return message ? "Permanent Redirect" : "308";

    // 4XX
    case HttpBadRequest:
        return message ? "Bad Request" : "400";
    case HttpUnauthorized:
        return message ? "Unauthorized" : "401";
    case HttpPaymentRequired:
        return message ? "Payment Required" : "402";
    case HttpForbidden:
        return message ? "Forbidden" : "403";
    case HttpNotFound:
        return message ? "Not Found" : "404";
    case HttpMethodNotAllowed:
        return message ? "Method Not Allowed" : "405";
    case HttpNotAcceptable:
        return message ? "Not Acceptable" : "406";
    case HttpProxyAuthenticationRequired:
        return message ? "Proxy Authentication Required" : "407";
    case HttpRequestTimeout:
        return message ? "Request Timeout" : "408";
    case HttpConflict:
        return message ? "Conflict" : "409";
    case HttpGone:
        return message ? "Gone" : "410";
    case HttpLengthRequired:
        return message ? "Length Required" : "411";
    case HttpPreconditionFailed:
        return message ? "Precondition Failed" : "412";
    case HttpContentTooLarge:
        return message ? "Content Too Large" : "413";
    case HttpURITooLong:
        return message ? "URI Too Long" : "414";
    case HttpUnsupportedMediaType:
        return message ? "Unsupported Media Type" : "415";
    case HttpRangeNotSatisfiable:
        return message ? "Range Not Satisfiable" : "416";
    case HttpExpectationFailed:
        return message ? "Expectation Failed" : "417";
    case HttpImATeapot:
        return message ? "I'm a teapot" : "418";
    case HttpMisdirectedRequest:
        return message ? "Misdirected Request" : "421";
    case HttpUnprocessableContent:
        return message ? "Unprocessable Content" : "422";
    case HttpLocked:
        return message ? "Locked" : "423";
    case HttpFailedDependency:
        return message ? "Failed Dependency" : "424";
    case HttpTooEarly:
        return message ? "Too Early" : "425";
    case HttpUpgradeRequired:
        return message ? "Upgrade Required" : "426";
    case HttpPreconditionRequired:
        return message ? "Precondition Required" : "428";
    case HttpTooManyRequests:
        return message ? "Too Many Requests" : "429";
    case HttpRequestHeaderFieldsTooLarge:
        return message ? "Request Header Fields Too Large" : "431";
    case HttpUnavailableForLegalReasons:
        return message ? "Unavailable For Legal Reasons" : "451";

    // 5XX
    case HttpInternalServerError:
        return message ? "Internal Server Error" : "500";
    case HttpNotImplemented:
        return message ? "Not Implemented" : "501";
    case HttpBadGateway:
        return message ? "Bad Gateway" : "502";
    case HttpServiceUnavailable:
        return message ? "Service Unavailable" : "503";
    case HttpGatewayTimeout:
        return message ? "Gateway Timeout" : "504";
    case HttpHTTPVersionNotSupported:
        return message ? "HTTP Version Not Supported" : "505";
    case HttpVariantAlsoNegotiates:
        return message ? "Variant Also Negotiates" : "506";
    case HttpInsufficientStorage:
        return message ? "Insufficient Storage" : "507";
    case HttpLoopDetected:
        return message ? "Loop Detected" : "508";
    case HttpNotExtended:
        return message ? "Not Extended" : "510";
    case HttpNetworkAuthenticationRequired:
        return message ? "Network Authentication Required" : "511";

    default:
        return 0;
    }
}
