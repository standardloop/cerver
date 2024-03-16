#ifndef REQUEST_H
#define REQUEST_H

#define MAX_LINE_LENGTH 50
#include "../methods/methods.h"

/*
HEAD / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.79.1
Accept: wildcard
*/

/*
GET/HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.79.1
Accept: wildcard
Connection: Upgrade, HTTP2-Settings
Upgrade: h2c
HTTP2-Settings: AAMAAABkAAQCAAAAAAIAAAAA
*/

typedef struct
{
    enum HttpMethods method;
    float httpVerion; // example: 1.1
    char *host;       // localhost
    int port;         // 8080
    char **headers;   // FIXM: headers package?
    char *body;       // FIXME: optional?
} HttpRequest;

HttpRequest *
CreateHttpRequest(char *, size_t);

#endif
