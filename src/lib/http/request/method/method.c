#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "methods.h"
#include "./../../../util/util.h"

// char *HttpMethodToStr(enum HttpMethods method)
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

enum HttpMethods
ParseRequestMethod(char *buffer, size_t buffer_size)
{
    printf("[TRACE]: entering ParseRequestMethod\n");
    if (buffer_size == 0 || buffer_size > MAX_METHOD_LENGTH)
    {
        printf("\n[ERROR][4XX]: buffer_size is 0\n");
        return HttpFAKER;
    }
    PrintBuffer(buffer, buffer_size);
    return HttpFAKER;
}
