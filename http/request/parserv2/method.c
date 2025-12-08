#include "./parser.h"

extern char *HTTPMethodEnumToString(enum V2HTTPMethod method)
{
    switch (method)
    {
    case V2HTTPGET:
        return "GET";
    case V2HTTPHEAD:
        return "HEAD";
    case V2HTTPPOST:
        return "POST";
    case V2HTTPPUT:
        return "PUT";
    case V2HTTPDELETE:
        return "DELETE";
    case V2HTTPCONNECT:
        return "CONNECT";
    case V2HTTPOPTIONS:
        return "OPTIONS";
    case V2HTTPTRACE:
        return "TRACE";
    case V2HTTPPATCH:
        return "PATCH";
    case V2HTTPINVALID:
    default:
        return NULL;
    }
}

// FIXME strncmp
enum V2HTTPMethod HTTPStringToMethodEnum(char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        return V2HTTPGET;
    }
    else if (strcmp(method, "HEAD") == 0)
    {
        return V2HTTPHEAD;
    }
    else if (strcmp(method, "POST") == 0)
    {
        return V2HTTPPOST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return V2HTTPPUT;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        return V2HTTPDELETE;
    }
    else if (strcmp(method, "CONNECT") == 0)
    {
        return V2HTTPCONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0)
    {
        return V2HTTPOPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0)
    {
        return V2HTTPTRACE;
    }
    else if (strcmp(method, "PATCH") == 0)
    {
        return V2HTTPPATCH;
    }
    return V2HTTPINVALID;
}
