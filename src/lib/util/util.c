#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "./util.h"

void PrintBuffer(char *buffer, size_t buffer_size)
{
    char *buffer_ptr = buffer;
    size_t i = 0;
    while (i < buffer_size && buffer_ptr != NULL && *buffer_ptr != '\0' && *buffer_ptr != '\0')
    {
        printf("%c", *buffer_ptr);
        i++;
        buffer_ptr++;
    }
    buffer_ptr = NULL;
}

char *StrToLower(char *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    for (char *p = s; *p; p++)
    {
        *p = tolower(*p);
    }
    return s;
}
char *StrToUpper(char *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    for (char *p = s; *p; p++)
    {
        *p = toupper(*p);
    }

    return s;
}
