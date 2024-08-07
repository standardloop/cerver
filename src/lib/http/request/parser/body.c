#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./parser.h"
#include <standardloop/logger.h>
#include <standardloop/util.h>

#define CONTENT_TYPE_APPLICATION_JSON "application/json"
#define CONTENT_TYPE_X_WWW_FORM_URLENCODED "application/x-www-form-urlencoded"

JSON *ParseBody(char *content_type, char *buffer, u_int64_t content_length)
{
    if (content_type == NULL || buffer == NULL || content_length == 0)
    {
        Log(WARN, "invalid inputs to ParseBody");
        return NULL;
    }
    if (strcmp(content_type, CONTENT_TYPE_APPLICATION_JSON) == 0)
    {
        JSON *body = StringToJSON(buffer);
        if (body == NULL)
        {
            Log(WARN, "failed to parse body");
            return NULL;
        }
        return body;
    }
    // TODO: support more than just JSON body
    // else if (strcmp(content_type, CONTENT_TYPE_X_WWW_FORM_URLENCODED) == 0)
    // {
    // }
    else
    {
        Log(ERROR, "this server can only handle JSON data");
    }

    return NULL;
}
