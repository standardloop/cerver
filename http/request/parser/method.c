#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "parser.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>

char *HTTPMethodToStr(enum HTTPMethod method)
{
    switch (method)
    {
    case HTTPGET:
        return "GET";
    case HTTPHEAD:
        return "HEAD";
    case HTTPPOST:
        return "POST";
    case HTTPPUT:
        return "PUT";
    case HTTPDELETE:
        return "DELETE";
    case HTTPCONNECT:
        return "CONNECT";
    case HTTPOPTIONS:
        return "OPTIONS";
    case HTTPTRACE:
        return "TRACE";
    case HTTPPATCH:
        return "PATCH";
    case HTTPINVALID:
    default:
        return NULL;
    }
}

enum HTTPMethod HTTPStrToMethod(char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        return HTTPGET;
    }
    else if (strcmp(method, "HEAD") == 0)
    {
        return HTTPHEAD;
    }
    else if (strcmp(method, "POST") == 0)
    {
        return HTTPPOST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return HTTPPUT;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        return HTTPDELETE;
    }
    else if (strcmp(method, "CONNECT") == 0)
    {
        return HTTPCONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0)
    {
        return HTTPOPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0)
    {
        return HTTPTRACE;
    }
    else if (strcmp(method, "PATCH") == 0)
    {
        return HTTPPATCH;
    }
    return HTTPINVALID;
}

enum HTTPMethod ParseRequestMethod(char *buffer, size_t buffer_size)
{

    if (buffer_size == 0 || buffer_size > MAX_METHOD_LENGTH)
    {
        Log(WARN, "[4XX]: cannot parse HTTP Method\n");
        return HTTPINVALID;
    }

    size_t http_method_size = buffer_size;
    char suspected_http_method_str[http_method_size];
    suspected_http_method_str[http_method_size - 1] = NULL_CHAR;

    size_t char_count = 0;
    while (char_count < buffer_size - 1 && *buffer != NEWLINE_CHAR && *buffer != SPACE_CHAR && *buffer != NULL_CHAR)
    {
        suspected_http_method_str[char_count] = *buffer;
        buffer++;
        char_count++;
    }

    if (suspected_http_method_str[http_method_size - 1] != NULL_CHAR)
    {
        Log(ERROR, "placed a null term wasn't there any more for suspected_http_method_str");
        return HTTPINVALID;
    }

    enum HTTPMethod method = HTTPStrToMethod(suspected_http_method_str);
    return method;
}
