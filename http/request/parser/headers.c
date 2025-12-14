#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./parser.h"
#include <standardloop/logger.h>
#include <standardloop/util.h>
#include <standardloop/json.h>

JSONValue *ParseHeader(char *line_start)
{
    //  FIXME: http headers to be case insensitve
    if (line_start == NULL)
    {
        return NULL;
    }
    // PrintBuffer(line_start, 2, true);

    char *colon_ptr = strchr(line_start, COLON_CHAR);
    // FIXME error checking
    size_t header_key_size = (colon_ptr - line_start) + 1;

    char *header_key = malloc(sizeof(char) * header_key_size);
    if (header_key == NULL)
    {
        return NULL;
    }

    CopyStringServer(line_start, header_key, header_key_size, 0, true);
    header_key[header_key_size - 1] = NULL_CHAR;

    char *header_value_start_after_colon = colon_ptr + 2;

    char *header_value_end = header_value_start_after_colon;
    size_t header_value_size = 0;
    while (header_value_end != NULL && *header_value_end != CARRIAGE_CHAR && *header_value_end != NULL_CHAR)
    {
        header_value_end++;
        header_value_size++;
    }
    header_value_size++;

    char *header_value = malloc(sizeof(char) * header_value_size);
    if (header_value == NULL)
    {
        free(header_key);
        return NULL;
    }
    CopyStringServer(header_value_start_after_colon, header_value, header_value_size, 0, false);
    header_value[header_value_size - 1] = NULL_CHAR;

    return JSONValueInit(JSONSTRING_t, (char *)header_value, header_key);
}
