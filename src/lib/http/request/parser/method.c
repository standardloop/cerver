#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "parser.h"
#include "./../../../util/util.h"
#include <standardloop/logger.h>

char *HttpMethodToStr(enum HttpMethod method)
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
    case HttpFAKE:
    default:
        return NULL;
    }
}

enum HttpMethod HttpStrToMethod(char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        return HttpGET;
    }
    else if (strcmp(method, "HEAD") == 0)
    {
        return HttpHEAD;
    }
    else if (strcmp(method, "POST") == 0)
    {
        return HttpPOST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return HttpPUT;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        return HttpDELETE;
    }
    else if (strcmp(method, "CONNECT") == 0)
    {
        return HttpCONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0)
    {
        return HttpOPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0)
    {
        return HttpTRACE;
    }
    else if (strcmp(method, "PATCH") == 0)
    {
        return HttpPATCH;
    }
    return HttpFAKE;
}

enum HttpMethod ParseRequestMethod(char *buffer, size_t buffer_size)
{

    if (buffer_size == 0 || buffer_size > MAX_METHOD_LENGTH)
    {
        Log(WARN, "[4XX]: cannot parse HTTP Method\n");
        return HttpFAKE;
    }

    size_t http_method_size = buffer_size;
    char suspected_http_method_str[http_method_size];
    suspected_http_method_str[http_method_size] = '\0';

    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != '\n' && *buffer != SPACE_CHAR && *buffer != '\0')
    {
        suspected_http_method_str[char_count] = *buffer;
        buffer++;
        char_count++;
    }

    if (suspected_http_method_str[http_method_size] != '\0')
    {
        Log(ERROR, "placed a null term wasn't there any more for suspected_http_method_str");
        return HttpFAKE;
    }

    enum HttpMethod method = HttpStrToMethod(suspected_http_method_str);
    return method;
}
