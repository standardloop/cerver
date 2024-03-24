#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "method.h"
#include "./../../../util/util.h"
#include "./../../../logger.h"

// char *HttpMethodToStr(enum HttpMethod method)
// {
//     char *method_str = NULL;
//     size_t length = 0;
//     switch (method)
//     {
//     case HttpGET:
//         length = 3 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "GET", length);
//         }
//         break;
//     case HttpHEAD:
//         length = 4 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "HEAD", length);
//         }
//         break;
//     case HttpPOST:
//         length = 4 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "POST", length);
//         }
//         break;
//     case HttpPUT:
//         length = 3 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "PUT", length);
//         }
//         break;
//     case HttpDELETE:
//         length = 6 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "DELETE", length);
//         }
//         break;
//     case HttpCONNECT:
//         length = 7 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "CONNECT", length);
//         }
//         break;
//     case HttpOPTIONS:
//         length = 7 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "OPTIONS", length);
//         }
//         break;
//     case HttpTRACE:
//         length = 5 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "TRACE", length);
//         }
//         break;
//     case HttpPATCH:
//         length = 5 + 1;
//         method_str = malloc(sizeof(char) * length);
//         if (method_str != NULL)
//         {
//             memcpy(method_str, "PATCH", length);
//         }
//         break;
//     case HttpFAKER:
//     default:
//         method_str = NULL;
//     }
//     return method_str;
// }

// TODO: do I need to allocate memory for this?
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
    case HttpFAKER:
    default:
        return NULL;
    }
}

enum HttpMethod HttpStrToMethod(char *method)
{
    // QUESTION, should it be "GET\0"?
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
    return HttpFAKER;
}

enum HttpMethod
ParseRequestMethod(char *buffer, size_t buffer_size)
{
    if (buffer_size == 0 || buffer_size > MAX_METHOD_LENGTH)
    {
        (void)Log(WARN, "[4XX]: cannot parse HTTP Method\n");
        return HttpFAKER;
    }

    size_t http_method_size = buffer_size + 1; // '\0'
    char suspected_http_method_str[http_method_size];
    suspected_http_method_str[http_method_size] = '\0';

    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != '\n' && *buffer != ' ' && *buffer != '\0' && *buffer != '/')
    {
        suspected_http_method_str[char_count] = *buffer;
        buffer++;
        char_count++;
    }
    enum HttpMethod method = HttpStrToMethod(suspected_http_method_str);
    return method;
}
