#ifndef RESPONSE_H
#define RESPONSE_H

#include "./codes.h"

typedef struct
{
    enum HttpCode response_code;
    int client_socket;
    char **headers;
    char *body;
} HttpResponse;

void FreeHttpResponse(HttpResponse *);
HttpResponse *CreateHttpResponse();
char *HttpResponseToString(HttpResponse *);

#endif
