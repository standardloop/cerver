#ifndef METHODS_H
#define METHODS_H

#define MAX_METHOD_LENGTH 10
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
char *HttpMethodStr(enum HttpMethods);

#endif
