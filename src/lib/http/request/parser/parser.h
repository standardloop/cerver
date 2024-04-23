#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

// HOST
char *ParseHost(char *, size_t);

// METHOD
#define MAX_METHOD_LENGTH 10
#define NUMBER_OF_METHODS 10

enum HttpMethod
{
    HttpGET,
    HttpHEAD,
    HttpPOST,
    HttpPUT,
    HttpDELETE,
    HttpCONNECT,
    HttpOPTIONS,
    HttpTRACE,
    HttpPATCH,
    HttpFAKE,
};
char *HttpMethodToStr(enum HttpMethod);
enum HttpMethod HttpStrToMethod(char *);
enum HttpMethod ParseRequestMethod(char *, size_t);

// PATH
#define MAX_PATH_SIZE 50

char *ParseRequestPath(char *, size_t);

// PORT
#define ERROR_PORT -1
#define MAX_PORT_LENGTH 5

int ParsePort(char *, size_t);

// QUERY
char *ParseQuery(char *, size_t);

// VERSION
#define HTTP_VERSION_PREFIX "HTTP/"

char *ParseHttpVersion(char *, size_t);

// HEADERS
#include "./../../../map/map.h"
Map *ParseHeaders(char *, size_t);

#endif