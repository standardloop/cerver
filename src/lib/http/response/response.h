#ifndef RESPONSE_H
#define RESPONSE_H

#include "./codes.h"

typedef struct
{
    int client_socket;
    enum HttpCode response_code;
    HashMap *headers;
    char *body; // FIXME
} HttpResponse;

void FreeHttpResponse(HttpResponse *);
HttpResponse *CreateHttpResponse();
char *HttpResponseToString(HttpResponse *);

#endif
