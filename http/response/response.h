#ifndef RESPONSE_H
#define RESPONSE_H

#include "./codes.h"

typedef struct
{
    int client_socket;
    char *version; // 1.1
    enum HTTPCode response_code;
    HashMap *headers;
    char *body; // FIXME
} HTTPResponse;

void FreeHTTPResponse(HTTPResponse *);
HTTPResponse *CreateHTTPResponse();
char *HTTPResponseToString(HTTPResponse *);

#endif
