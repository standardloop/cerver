#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "parser.h"
#include "./../../../util/util.h"
#include <standardloop/logger.h>

char *ParseRequestPath(char *buffer, size_t buffer_size)
{
    if (buffer_size == 0 || buffer_size > MAX_PATH_SIZE)
    {
        Log(WARN, "[4XX]: cannot parse HTTP path\n");
        return NULL;
    }

    char *buffer_start = buffer;

    size_t http_path_size = buffer_size;
    char *suspected_http_path_str = (char *)malloc(sizeof(char) * http_path_size);
    if (suspected_http_path_str == NULL)
    {
        Log(WARN, "[5XX]: cannot parse HTTP path memory error\n");
        return NULL;
    }
    char *suspected_http_path_str_start = suspected_http_path_str;
    *(suspected_http_path_str + http_path_size) = '\0';
    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != SPACE_CHAR && *buffer != NULL_CHAR && *buffer != QUESTION_CHAR)
    {
        *suspected_http_path_str = *buffer;
        buffer++;
        suspected_http_path_str++;
        char_count++;
    }
    // reset to start
    buffer = buffer_start;
    suspected_http_path_str = suspected_http_path_str_start;

    if (*(suspected_http_path_str + http_path_size) != '\0')
    {
        Log(ERROR, "http path string was not null terminated\n");
        free(suspected_http_path_str);
        return NULL;
    }
    return suspected_http_path_str;
}
