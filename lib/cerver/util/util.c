

#include <stdio.h>
#include "./util.h"

void PrintBuffer(char *buffer)
{
    char *buffer_ptr = buffer;
    while (buffer_ptr != NULL && *buffer_ptr != '\0')
    {
        printf("%c", *buffer_ptr);
        buffer_ptr++;
    }
    buffer_ptr = NULL;
}
