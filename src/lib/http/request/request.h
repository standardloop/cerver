#ifndef REQUEST_H
#define REQUEST_H

#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 50
#define ERROR_SIZE_T 0

#include "./parser/method.h"
#include "./../response/codes/codes.h"

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
    enum HttpCode early_resp_code;
    char *error_message;
    size_t message_length;
} Error;

typedef struct
{
    enum HttpMethod method;
    char *path;
    char *query;   // FIXME probably want to use a map for this
                   // but tempoarility just store it all in a string
    char *version; // 1.1
    char *host;    // localhost
    int port;      // 8080
    Error *error;  // WIP possible

    char **headers; // FIXM: map
    char *body;     // FIXME: optional? maybe **
} HttpRequest;

HttpRequest *
ParseHttpRequest(char *, size_t);

void FreeHttpRequest(HttpRequest *);
void PrintHttpRequest(HttpRequest *);

#endif
