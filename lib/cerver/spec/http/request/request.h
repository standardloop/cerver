#ifndef REQUEST_H
#define REQUEST_H

#include "../methods/methods.h"

typedef struct Request
{
    int http_method;
    char *target;   //
    char **headers; // FIXM: headers package?
    char *body;     // FIXME: optional?
};

#endif
