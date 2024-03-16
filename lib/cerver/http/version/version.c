#include "./version.h"

float ParseHttpVersion(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        return 0.0;
    }
    char *temp_ptr = buffer + 1; // '\n is always first line'
    size_t char_count = 0;

    while (char_count < buffer_size && *temp_ptr != '\0' && *temp_ptr != '\n')
    {
        printf("%c", *temp_ptr);
        temp_ptr++;
        char_count++;
    }
    temp_ptr = NULL;
    return 1.1;
}
