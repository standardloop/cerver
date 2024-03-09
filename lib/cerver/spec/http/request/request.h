#ifndef REQUEST_H
#define REQUEST_H

#include "../methods/methods.h"

typedef struct HttpRequest
{
    HttpMethods http_method;
    char *target;   //
    char **headers; // FIXM: headers package?
    char *body;     // FIXME: optional?
};

#endif
