#ifndef REQUEST_H
#define REQUEST_H

#include <standardloop/json.h>

#include "./parser/parser.h"
#include "./../response/codes.h"

#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 50
#define ERROR_SIZE_T 0

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
//     enum HttpCode bail_resp_code;
//     char *error_message;
//     size_t message_length;
// } Error;

typedef struct
{
    int client_socket;
    enum HttpMethod method;
    char *path;
    HashMap *query_params;
    char *version;  // 1.1
    char *host;     // localhost
    u_int16_t port; // 8080
    enum HttpCode bail_resp_code;

    HashMap *headers;
    HashMap *body;
    HashMap *path_params;

} HttpRequest;

HttpRequest *CreateParsedHttpRequest(char *, size_t);

void FreeHttpRequest(HttpRequest *);
void PrintHttpRequest(HttpRequest *);

#endif
