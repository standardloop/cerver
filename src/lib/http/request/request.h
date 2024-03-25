#ifndef REQUEST_H
#define REQUEST_H

#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 50
#define ERROR_SIZE_T 0

#include "./parser/method.h"

/*
HEAD / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.79.1
Accept: wildcard
*/

/*
GET / HTTP/1.1
Host: localhost:8080
User-Agent: curl/7.79.1
Accept: wildcard
Connection: Upgrade, HTTP2-Settings
Upgrade: h2c
HTTP2-Settings: AAMAAABkAAQCAAAAAAIAAAAA
*/


typedef struct
{
    enum HttpMethod method;
    char *path_and_query; // FIXME, seperate this at some point
    char *version;
    char *host;     // localhost
    int port;       // 8080
    char **headers; // FIXM: headers package?
    char *body;     // FIXME: optional? maybe **
} HttpRequest;

HttpRequest *
ParseHttpRequest(char *, size_t);

void FreeHttpRequest(HttpRequest *);
void PrintHttpRequest(HttpRequest *);
void HandleRequest(int);
#endif
