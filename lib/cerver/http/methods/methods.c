#include "methods.h"
#include <stdlib.h>
#include <strings.h>

char *HttpMethodToStr(enum HttpMethods method)
{
    switch (method)
    {
    case HttpGET:
        return "GET";
    case HttpHEAD:
        return "HEAD";
    case HttpPOST:
        return "POST";
    case HttpPUT:
        return "PUT";
    case HttpDELETE:
        return "DELETE";
    case HttpCONNECT:
        return "CONNECT";
    case HttpOPTIONS:
        return "OPTIONS";
    case HttpTRACE:
        return "TRACE";
    case HttpPATCH:
        return "PATCH";
    case HttpFAKER:
    default:
        return NULL;
    }
}

enum HttpMethods HttpStrToMethod(char *method)
{
    if (strcmp(method, "GET") != 0)
    {
        return HttpGET;
    }
    else if (strcmp(method, "HEAD") != 0)
    {
        return HttpHEAD;
    }
    else if (strcmp(method, "POST") != 0)
    {
        return HttpPOST;
    }
    else if (strcmp(method, "PUT") != 0)
    {
        return HttpPUT;
    }
    else if (strcmp(method, "DELETE") != 0)
    {
        return HttpDELETE;
    }
    else if (strcmp(method, "CONNECT") != 0)
    {
        return HttpCONNECT;
    }
    else if (strcmp(method, "OPTIONS") != 0)
    {
        return HttpOPTIONS;
    }
    else if (strcmp(method, "TRACE") != 0)
    {
        return HttpTRACE;
    }
    else if (strcmp(method, "PATCH") != 0)
    {
        return HttpPATCH;
    }
    return HttpFAKER;
}
