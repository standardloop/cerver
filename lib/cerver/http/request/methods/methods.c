#include "methods.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

size_t getMethodStrSize(char *, size_t);
void substringUntil(char *, char *, int);
enum HttpMethods extractHttpMethod(char *, size_t);

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

size_t getMethodStrSize(char *buffer, size_t buffer_size)
{
    // FIXME: ADD ERROR LOGIC
    char *temp_ptr = buffer;
    size_t size = 0;
    // ((sizeof(char)) * size)
    while (size < MAX_METHOD_LENGTH && size < buffer_size && *temp_ptr != '/' && *temp_ptr != '\0')
    {
        temp_ptr++;
        size++;
    }
    temp_ptr = NULL;
    return size;
}

// FIXME: util or built in function for this?

void substringUntil(char *original, char *substr, int index)
{
    // FIXME index?
    for (int i = 0; i < (index - 1); i++)
    {
        substr[i] = original[i];
    }
    // printf("\n[TRACE]: substringUntil: %s\n", substr);
}

enum HttpMethods
extractHttpMethod(char *buffer, size_t size)
{
    // FIXME: do I need null terminate
    char *buffer_substr = malloc(sizeof(char) * size);
    if (buffer_substr != NULL)
    {
        (void)substringUntil(buffer, buffer_substr, size / sizeof(char)); // modifies buffer_substr
        enum HttpMethods method = HttpStrToMethod(buffer_substr);
        free(buffer_substr);
        return method;
    }
    return HttpFAKER;
}

enum HttpMethods
ParseRequestMethod(char *buffer, size_t buffer_size)
{
    // printf("[TRACE]: entering ParseRequestMethod\n");
    size_t size = getMethodStrSize(buffer, buffer_size);
    if (size == 0)
    {
        return HttpFAKER;
    }
    return extractHttpMethod(buffer, size);
}
