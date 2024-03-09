#ifndef METHODS_H
#define METHODS_H
char *HttpMethodStr(int method);
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
};
#endif
