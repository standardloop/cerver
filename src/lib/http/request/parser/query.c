#include <stdio.h>
#include <stdlib.h>

#include "./query.h"
#include "./../../../util/util.h"
#include "./../../../logger.h"

char *ParseQuery(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        (void)Log(WARN, "\n[4XX]: buffer input for ParseQuery is NULL \n");
        return NULL;
    }
    char *buffer_start = buffer;

    size_t http_query_size = buffer_size;
    char *suspected_http_query_str = (char *)malloc(sizeof(char) * http_query_size);
    if (suspected_http_query_str == NULL)
    {
        (void)Log(WARN, "[5XX]: cannot parse HTTP query memory error\n");
        return NULL;
    }
    char *suspected_http_query_str_start = suspected_http_query_str;
    *(suspected_http_query_str + http_query_size) = '\0';
    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != SPACE_CHAR && *buffer != '\0')
    {
        *suspected_http_query_str = *buffer;
        buffer++;
        suspected_http_query_str++;
        char_count++;
    }
    // reset to start
    buffer = buffer_start;
    suspected_http_query_str = suspected_http_query_str_start;

    if (*(suspected_http_query_str + http_query_size) != '\0')
    {
        (void)Log(ERROR, "http query string was not null terminated\n");
        free(suspected_http_query_str);
        return NULL;
    }
    return suspected_http_query_str;
}
