#include "methods.h"
#include <stdlib.h>

char *HttpMethoEnumToString(int method)
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
    default:
        return NULL;
    }
}