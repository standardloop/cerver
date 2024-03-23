#include "./port.h"
#include <stdbool.h>

int ParsePort(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        printf("\n[ERROR][4XX]: invalid buffer for parsing port");
        return ERROR_PORT;
    }
    char *temp_ptr = buffer;
    char *start = buffer;
    char *end = buffer;
    size_t size = 0;
    int colon_count = 0;

    bool stop = false;
    while (!stop && size < buffer_size && *temp_ptr != '\0' && *temp_ptr != '\n')
    {
        if (*temp_ptr == ':')
        {
            colon_count++;
            if (colon_count == 2)
            {
                start = temp_ptr;
            }
        }
        temp_ptr++;
        size++;
    }
    start++; // move after the colon
    end = temp_ptr;

    size_t port_string_size = (end + 1) - start;

    char *port_string = malloc(sizeof(char) * port_string_size);

    if (port_string == NULL)
    {
        printf("\n[ERROR][5XX]: couldn't allocate memory for port string");
        return ERROR_PORT;
    }

    size_t char_count = 0;
    while (char_count < port_string_size && start != end)
    {
        port_string[char_count] = *start;
        char_count++;
        start++;
    }
    port_string[port_string_size] = '\0';
    int port = atoi(port_string);
    if (port == 0)
    {
        printf("\n[ERROR][4XX]: port is 0 :P");
        return ERROR_PORT;
    }
    free(port_string);
    return port;
}
