#ifndef METHODS_H
#define METHODS_H

#define MAX_METHOD_LENGTH 10
#include <stdlib.h>
#include <stdio.h>
enum HttpMethods
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
char *HttpMethodToStr(enum HttpMethods);
enum HttpMethods HttpStrToMethod(char *);
enum HttpMethods ParseRequestMethod(char *, size_t);
#endif
