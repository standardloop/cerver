#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./parser.h"
#include <standardloop/logger.h>
#include <standardloop/util.h>

#define CONTENT_TYPE_APPLICATION_JSON "application/json"
#define CONTENT_TYPE_X_WWW_FORM_URLENCODED "application/x-www-form-urlencoded"

HashMap *ParseBody(char *content_type, char *buffer, size_t buffer_size)
{
    if (content_type == NULL || buffer == NULL || buffer_size == 0)
    {
        return NULL;
    }
    return NULL;
    if (strcmp(content_type, CONTENT_TYPE_APPLICATION_JSON) == 0)
    {
        JSON *content = StringToJSON(NULL);
        if (content == NULL)
        {
        }
    }
    else if (strcmp(content_type, CONTENT_TYPE_X_WWW_FORM_URLENCODED) == 0)
    {
    }

    return NULL;
}
