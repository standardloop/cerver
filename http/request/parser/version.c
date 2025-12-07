#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "./parser.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>

char *ParseHTTPVersion(char *buffer, size_t buffer_size)
{
    // HTTP/1.1
    if (buffer == NULL || buffer_size == 0)
    {
        Log(ERROR, "ParseHTTPVersion buffer input error\n");
        return NULL;
    }

    char *slash_point = strchr(buffer, FORWARDLASH_CHAR);

    if (*slash_point != FORWARDLASH_CHAR)
    {
        Log(WARN, "[4XX]: cannot parse HTTP version slash not found\n");
        return NULL;
    }

    size_t version_prefix_size = (slash_point + 1) - buffer;
    if (version_prefix_size != strlen(HTTP_VERSION_PREFIX))
    {
        Log(WARN, "[4XX]: HTTP version prefix no good\n");
        return NULL;
    }

    char *buffer_start = slash_point + 1;
    size_t http_version_size = buffer_size - ((slash_point + 1) - buffer);
    http_version_size++; // NULL_CHAR

    char *http_version_str = (char *)malloc(sizeof(char) * http_version_size);
    if (http_version_str == NULL)
    {
        Log(WARN, "[4XX]: cannot parse HTTP version memory error\n");
        return NULL;
    }

    char *http_version_str_start = http_version_str;
    *(http_version_str + http_version_size - 1) = NULL_CHAR;

    size_t char_count = 0;
    buffer = buffer_start;
    while (char_count < http_version_size && *buffer != NULL_CHAR)
    {
        *http_version_str = *buffer;

        buffer++;
        http_version_str++;
        char_count++;
    }

    buffer = buffer_start;
    http_version_str = http_version_str_start;
    if (*(http_version_str + http_version_size - 1) != NULL_CHAR)
    {
        Log(ERROR, "http path string was not null terminated\n");
        free(http_version_str);
        return NULL;
    }
    return http_version_str;
}
