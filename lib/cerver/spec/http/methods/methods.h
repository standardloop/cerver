#ifndef METHODS_H
#define METHODS_H

char *HttpMethoEnumToString(int method);

enum HttpMethods
{
    HttpGET = 0,
    HttpHEAD = 1,
    HttpPOST = 2,
    HttpPUT = 3,
    HttpDELETE = 4,
    HttpCONNECT = 5,
    HttpOPTIONS = 6,
    HttpTRACE = 7,
    HttpPATCH = 8,
};
#endif
