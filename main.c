#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <standardloop/logger.h>

#include "./cerver.h"
#include "./http/handler.h"
#include "./http/request/parserv2/parser.h"

#include <standardloop/util.h>

void foo(const HTTPRequest *, HTTPResponse *);
void bar(const HTTPRequest *, HTTPResponse *);
void fooID(const HTTPRequest *, HTTPResponse *);
void fooStatic(const HTTPRequest *, HTTPResponse *);

void foo(const HTTPRequest *request, HTTPResponse *response)
{
    // Log(TRACE, "[JOSH]: entering special test function\n");
    // char *accepted_types = MapGet(request->headers, "Accept");
    // if (accepted_types == NULL)
    // {
    // }
    if (request == NULL || response == NULL)
    {
        return;
    }
    response->response_code = HTTPOK;
    response->body = QuickAllocatedString("Hello!");
    SendResponse(response);
}

void fooID(const HTTPRequest *request, HTTPResponse *response)
{
    // char *path_param_id = GetPathParam(request->path_params, "id");
    JSONValue *path_param_id_obj = HashMapGet(request->path_params, "id");
    if (path_param_id_obj == NULL)
    {
        const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
        (void)write(request->client_socket, hello, strlen(hello));
        return;
    }
    char *path_param_id = path_param_id_obj->value;
    if (path_param_id != NULL)
    {
        printf("\npath_param_id: %s\n", path_param_id);
    }

    Log(TRACE, "[JOSH]: entering special test ID function\n");
    if (request == NULL || response == NULL)
    {
        return;
    }
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    (void)write(request->client_socket, hello, strlen(hello));
}

void fooStatic(const HTTPRequest *request, HTTPResponse *response)
{
    // Log(TRACE, "[JOSH]: entering special test function\n");
    JSONValue *accepted_types_obj = HashMapGet(request->path_params, "Accept");
    char *accepted_types = accepted_types_obj->value;
    if (accepted_types == NULL)
    {
    }
    if (request == NULL || response == NULL)
    {
        return;
    }
    HandleStaticPath(request->client_socket, "/static/foo.html");
    return;
}

int main(void)
{
    SetLogLevel(StringToLogLevel(GetEnv("LOG_LEVEL", "TRACE")));
    // char *http_request = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
    char *http_request =
        "POST /api/data HTTP/1.1\r\n"
        "Host: example.com:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";
    //  char *http_request = "GET /index.html HTTP/1.1\r\n";
    // HTTPLexerDebugTest(http_request, false);

    HTTPLexer *lexer = HTTPLexerInit(http_request);
    HTTPParser *parser = HTTPParserInit(lexer);
    HTTPRequest *request = ParseHTTP(parser);

    FreeHTTPRequest(request);
    return 0;
}

// int main(void)
// {
//     int port = atoi(GetEnv("PORT", "8080"));
//     int num_threads = atoi(GetEnv("NUM_THREADS", "4"));
//     int buffer_size = atoi(GetEnv("BUFFER_SIZE", "100"));

//     SetLogLevel(StringToLogLevel(GetEnv("LOG_LEVEL", "TRACE")));
//     // SetLogLevel(StringToLogLevel(GetEnv("LOG_LEVEL", "ERROR")));

//     Cerver *server = InitCerver(port, num_threads, buffer_size);

//     (void)AddRouteToTable(server->router->get, "/foo", foo);
//     // (void)AddRouteToTable(server->router->get, "/foo/{id=int}/bar/{name=string}", fooID);

//     // WIP support *
//     //(void)AddRouteToTable(server->router->get, "/bar/.*", bar);

//     StartCerver(server);

//     return EXIT_SUCCESS;
// }
