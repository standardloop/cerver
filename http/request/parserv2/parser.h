#ifndef SL_CERVER_PARSER_H
#define SL_CERVER_PARSER_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
    GET /index.html HTTP/1.1\r\n
    Host: example.com\r\n
    \r\n
*/

// const char **http_methods = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};

#define HTTP_TOKEN_SPACE_STRING " "

#define HTTP_GET_STRING "GET"
#define HTTP_GET_LENGTH 3
#define HTTP_PUT_STRING "PUT"
#define HTTP_PUT_LENGTH 3

#define HTTP_POST_STRING "POST"
#define HTTP_POST_LENGTH 4
#define HTTP_HEAD_STRING "HEAD"
#define HTTP_HEAD_LENGTH 4

#define HTTP_PATCH_STRING "PATCH"
#define HTTP_PATCH_LENGTH 5
#define HTTP_TRACE_STRING "TRACE"
#define HTTP_TRACE_LENGTH 5

#define HTTP_DELETE_STRING "DELETE"
#define HTTP_DELETE_LENGTH 6

#define HTTP_OPTIONS_STRING "OPTIONS"
#define HTTP_OPTIONS_LENGTH 7
#define HTTP_CONNECT_STRING "CONNECT"
#define HTTP_CONNECT_LENGTH 7

#define HTTP_CRLF_STRING "\r\n"
#define HTTP_COLON_STRING ":"

// add these to the cerver init

#define MAX_URI_LENGTH 2046

#define MAX_VERSION_LENGTH 10 // HTTP/1.1 // 8?

#define MAX_HEADER_LENGTH 128 // FIXME arbitrary

#define MAX_BODY_LENGTH 2147483647 // FIXME arbitrary

enum HTTPTokenType
{
    HTTPTokenMethod,
    HTTPTokenSpace,
    HTTPTokenURI,
    HTTPTokenVersion,
    HTTPTokenCRLF,
    HTTPTokenNewline,
    HTTPTokenCarriage,
    HTTPTokenHeaderName, // not used
    HTTPTokenPort,       // not used
    HTTPTokenColon,
    HTTPTokenHeaderValue, // not used
    HTTPTokenString,      // FIXME, how to diff between headername or headervalue
    HTTPTokenBody,
    HTTPTokenEOF,
    HTTPTokenIllegal,
};

typedef struct
{
    enum HTTPTokenType type;
    u_int32_t start;
    u_int32_t end;
    u_int32_t line;
    char *literal;
} HTTPToken;

typedef struct
{
    char *input;
    u_int32_t input_len;
    char current_char;
    u_int32_t position;
    u_int32_t read_position;
    u_int32_t line;
    enum HTTPTokenType last_token;
    struct error
    {
        bool is_server;
        bool is_client;
        char *message;
    } error;
    bool double_crlf;
} HTTPLexer;

extern HTTPLexer *HTTPLexerInit(char *);
extern void FreeHTTPLexer(HTTPLexer *);
extern void HTTPLexerDebugTest(char *, bool);
extern HTTPToken *HTTPLex(HTTPLexer *);
extern HTTPToken *NewHTTPToken(enum HTTPTokenType, u_int32_t, u_int32_t, u_int32_t, char *);
extern void FreeHTTPToken(HTTPToken *);

extern void PrintHTTPToken(HTTPToken *, bool);

///

typedef struct
{
    HTTPLexer *lexer;
    HTTPToken *previous_token;
    HTTPToken *current_token;
    HTTPToken *peek_token;
    bool input_error;
    bool memory_error;
    char *error_message;
} HTTPParser;

extern HTTPParser *HTTPParserInit(HTTPLexer *);
extern void PrintHTTPParserError(HTTPParser *);
extern void FreeHTTPParser(HTTPParser *);
extern void PrintHTTPParserErrorLine(HTTPParser *);
extern void NextHTTPToken(HTTPParser *);

// METHOD

enum V2HTTPMethod
{
    V2HTTPGET,
    V2HTTPHEAD,
    V2HTTPPOST,
    V2HTTPPUT,
    V2HTTPDELETE,
    V2HTTPCONNECT,
    V2HTTPOPTIONS,
    V2HTTPTRACE,
    V2HTTPPATCH,
    V2HTTPINVALID,
};

extern char *HTTPMethodEnumToString(enum V2HTTPMethod);
enum V2HTTPMethod HTTPStringToMethodEnum(char *);

#endif
