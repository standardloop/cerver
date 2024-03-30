#ifndef METHOD_H
#define METHOD_H

#define MAX_METHOD_LENGTH 10
#define NUMBER_OF_METHODS 10

#include <stdlib.h>
#include <stdio.h>
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
    HttpFAKER,
};
char *HttpMethodToStr(enum HttpMethod);
enum HttpMethod HttpStrToMethod(char *);
enum HttpMethod ParseRequestMethod(char *, size_t);
#endif
