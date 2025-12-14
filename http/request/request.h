#ifndef REQUEST_H
#define REQUEST_H

#include <standardloop/json.h>

#include "./parser/parser.h"

#include "./parserv2/parser.h"

#include "./../response/codes.h"

#define BUFFER_SIZE 1024
#define MAX_LINE_LENGTH 50
#define ERROR_SIZE_T 0

#define DEFAULT_HTTP_PORT 80
#define DEFAULT_HTTPS_PORT 443

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
//     enum HTTPCode bail_resp_code;
//     char *error_message;
//     size_t message_length;
// } Error;

enum HTTPRequestBodyTypes
{
    HTTPRequestBodyJSON,
    HTTPRequestBodyPlain,
    // HTTPRequestBodyOctet,
    // HTTPRequestBodyXML,
};

typedef struct
{
    int client_socket;
    enum HTTPMethod method;
    enum V2HTTPMethod method_v2;
    char *path;
    char *uri;
    HashMap *query_params;
    char *version;  // 1.1
    char *host;     // localhost
    u_int16_t port; // 8080
    struct host_v2
    {
        char *hostname;
        u_int16_t port_number;
    } host_v2;
    enum HTTPCode bail_resp_code;
    HashMap *headers;
    JSON *body; // for now only support JSON body
    struct body_v2
    {
        void *literal;
        enum HTTPRequestBodyTypes type;
    } body_v2;
    struct error_v2
    {
        bool is_server;
        bool is_client;
        char *message;
        enum HTTPCode bail_resp_code;
    } error_v2;
    bool tls;
    HashMap *path_params;

} HTTPRequest;

HTTPRequest *CreateParsedHTTPRequest(char *, size_t);
extern HTTPRequest *ParseHTTPRequest(HTTPParser *);

void FreeHTTPRequest(HTTPRequest *);
void PrintHTTPRequest(HTTPRequest *);

#endif
