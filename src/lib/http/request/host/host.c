#include "./host.h"
#include "../../../util/util.h"
#include <stdbool.h>

//: localhost:
char *substrBetweenColons(char *start, char *end, size_t buffer_size)
{
    // printf("\n[INFO]: entering substrBetweenColons\n");
    //(void)PrintBuffer(start, buffer_size);
    //
    if (*(start) != ':')
    {
        printf("\n[ERROR][4XX]: host is NULL substrBetweenColons\n");
        return NULL;
    }
    // localhost:
    start++;
    while (*(start) == ' ')
    {
        start++;
        buffer_size--;
    }
    // localhost:
    char *host = malloc(sizeof(char) * buffer_size);
    if (host == NULL)
    {
        printf("\n[ERROR][5XX]: host is NULL substrBetweenColons\n");
        return NULL;
    }
    size_t iteration_count = 0;

    while (start != end && iteration_count < buffer_size)
    {
        host[iteration_count] = *start;
        iteration_count++;
        start++;
    }
    host[buffer_size] = '\0';
    return host;
}

char *ParseHost(char *buffer, size_t buffer_size)
{
    //(void)PrintBuffer(buffer);
    // Host: localhost:8080
    // printf("\n[INFO]: entering ParseHost\n");
    if (buffer == NULL || buffer_size == 0)
    {
        printf("\n[ERROR][4XX]: buffer input for ParseHost is NULL :O\n");
        return NULL;
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
            if (colon_count == 1)
            {
                start = temp_ptr;
            }
            if (colon_count == 2)
            {
                end = temp_ptr;
                stop = true;
            }
        }
        temp_ptr++;
        size++;
    }
    temp_ptr = NULL;
    size_t distance = end - start;
    distance++; // for '\0'
    //": localhost:8080"
    // printf("\n[JOSH]: distance: %d\n", (int)distance);
    if (*start != ':' && *end != ':')
    {
        printf("\n[ERROR][4XX]: cannot find colons\n");
        return NULL;
    }
    //(void)PrintBuffer(start, buffer_size);

    char *host = substrBetweenColons(start, end, distance);

    //(void)PrintBuffer(host, distance);
    if (host == NULL)
    {
        printf("\n[ERROR][4XX]: host is NULL after calling substrBetweenColons\n");
        return NULL;
    }
    return host;
}
