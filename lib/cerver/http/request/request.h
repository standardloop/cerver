#ifndef REQUEST_H
#define REQUEST_H

#include "../methods/methods.h"

typedef struct
{
    enum HttpMethods http_method;
    char *target;   //
    char **headers; // FIXM: headers package?
    char *body;     // FIXME: optional?
} HttpRequest;

HttpRequest *CreateHttpRequest();
long ParseRequestMethod(char *, size_t);
#endif
