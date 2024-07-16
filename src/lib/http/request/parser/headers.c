#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./parser.h"
#include <standardloop/logger.h>
#include <standardloop/util.h>

static void addHeaderFromLine(HashMap *, char *, size_t);

HashMap *ParseHeaders(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        Log(ERROR, "[ERROR][4XX]: invalid buffer for parsing headers");
        return NULL;
    }
    HashMap *headers = DefaultHashMapInit();
    if (headers == NULL)
    {
        return NULL;
    }
    char *buffer_start = buffer;
    char *line_start = buffer;
    while (buffer != NULL && *buffer != '\0')
    {
        if (*buffer == NEWLINE_CHAR)
        {
            char *line_without_carriage_return = buffer;
            line_without_carriage_return--;

            // remove carriage return from line
            while (*line_without_carriage_return == CARRIAGE_CHAR)
            {
                line_without_carriage_return--;
            }
            (void)addHeaderFromLine(headers, line_start, (size_t)(line_without_carriage_return - line_start));
            line_start = buffer + 1;
        }
        buffer++;
    }
    buffer = buffer_start;

    // char *accepted_types = MapGet(headers, "Accept");
    // if (accepted_types == NULL)
    // {
    //     MapAdd(headers, );
    // }
    return headers;
}

static void addHeaderFromLine(HashMap *headers, char *line_start, size_t line_size)
{
    //  FIXME
    //  http headers to be case insensitve
    if (headers == NULL || line_start == NULL)
    {
        return;
    }
    char *colon_ptr = strchr(line_start, COLON_CHAR);
    // FIXME error checking
    size_t header_key_size = (colon_ptr - line_start) + 1;

    char *header_key = malloc(sizeof(char) * header_key_size);
    if (header_key == NULL)
    {
        // return NULL;
        return;
    }
    *(header_key + header_key_size) = '\0';

    char *header_key_iterator = header_key;
    char *line_start_iterator = line_start;
    while (*line_start_iterator != COLON_CHAR &&
           *line_start_iterator != '\0' &&
           line_start_iterator != NULL)
    {
        *header_key_iterator = *line_start_iterator;
        header_key_iterator++;
        line_start_iterator++;
    }
    // char *header_value = colon_ptr + 1;
    // FIXME: not properly accounting for length (space char, carriage return, newline, etc)
    size_t header_value_size = line_size - header_key_size;
    char *header_value = malloc(sizeof(char) * header_value_size);
    if (header_value == NULL)
    {
        free(header_key);
        return;
        // return NULL;
    }
    *(header_value + header_value_size) = '\0';
    char *header_value_iterator = header_value;
    char *value_iterator = colon_ptr + 1;

    while (*value_iterator == SPACE_CHAR)
    {
        value_iterator++;
    }

    while (*value_iterator != NEWLINE_CHAR &&
           *value_iterator != CARRIAGE_CHAR &&
           *value_iterator != '\0' &&
           value_iterator != NULL)
    {
        *header_value_iterator = *value_iterator;
        header_value_iterator++;
        value_iterator++;
    }

    HashMapInsert(headers, JSONValueInit(STRING_t, header_value, header_key));
}
