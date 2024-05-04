#ifndef REQUEST_H
#define REQUEST_H

#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 50
#define ERROR_SIZE_T 0

#include "./parser/parser.h"
#include "./../response/codes.h"
#include "./../../structures/map/map.h"

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

// typedef struct
// {
//     enum HttpCode early_resp_code;
//     char *error_message;
//     size_t message_length;
// } Error;

typedef struct
{
    int client_socket;
    enum HttpMethod method;
    char *path;
    Map *query_params;
    char *version; // 1.1
    char *host;    // localhost
    int port;      // 8080
    enum HttpCode early_resp_code;

    Map *headers;
    char *body; // FIXME:
    Map *path_params;
} HttpRequest;

HttpRequest *
CreateParsedHttpRequest(char *, size_t);

void FreeHttpRequest(HttpRequest *);
void PrintHttpRequest(HttpRequest *);

#endif
