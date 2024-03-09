#ifndef RESPONSE_H
#define RESPONSE_H

#include "../codes/codes.h"


typedef struct Response
{
    HttpCodes response_code;
    char **headers; // FIXM: headers package?
    char *body;     // FIXME: optional?
};

#endif
