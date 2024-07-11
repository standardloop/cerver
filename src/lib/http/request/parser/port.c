#include <stdbool.h>

#include "./parser.h"
#include "./../../../util/util.h"
#include <standardloop/logger.h>

int ParsePort(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0 || buffer_size > MAX_PORT_LENGTH)
    {
        Log(ERROR, "[ERROR][4XX]: invalid buffer for parsing port");
        return ERROR_PORT;
    }

    // since we are going to convert the string to an int
    // we can store the string on the stack instead of heap

    size_t port_size = buffer_size;
    char port_str[port_size];

    port_str[port_size] = '\0';

    size_t char_count = 0;
    while (char_count < buffer_size && *buffer != '\0')
    {
        port_str[char_count] = *buffer;

        buffer++;
        char_count++;
    }
    if (port_str[port_size] != '\0')
    {
        Log(ERROR, "placed a null term wasn't there any more for port\n");
        return ERROR_PORT;
    }
    int port = atoi(port_str);
    if (port == 0)
    {
        return ERROR_PORT;
    }
    return port;
}
