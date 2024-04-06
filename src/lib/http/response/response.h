#ifndef RESPONSE_H
#define RESPONSE_H

#include "./codes.h"

typedef struct
{
    enum HttpCode response_code;
    char **headers;
    char *body;
} HttpResponse;

void FreeHttpResponse(HttpResponse *);

#endif
