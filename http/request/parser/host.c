#include <stdbool.h>

#include "./parser.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>

char *ParseHost(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        Log(WARN, "\n[4XX]: buffer input for ParseHost is NULL \n");
        return NULL;
    }

    size_t host_size = buffer_size;
    char *host_str = (char *)malloc(sizeof(char) * host_size);
    if (host_str == NULL)
    {
        Log(WARN, "[5XX]: cannot allocate memory to parse host\n");
        return NULL;
    }
    char *host_str_start = host_str;
    *(host_str + host_size) = '\0';

    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != '\0')
    {
        *host_str = *buffer;

        buffer++;
        host_str++;
        char_count++;
    }
    host_str = host_str_start;

    if (*(host_str + host_size) != '\0')
    {
        Log(ERROR, "placed a null term wasn't there any more for http host\n");
        free(host_str);
        return NULL;
    }

    return host_str;
}
