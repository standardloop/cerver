#include "./host.h"

// Host: localhost:8080
char *substrBetweenColons(char *start, char *end, size_t buffer_size)
{

    char *host = malloc(sizeof(char) * buffer_size);
    if (host == NULL)
    {
        return NULL;
    }
    size_t iteration_count = 0;
    while (start != end && iteration_count < buffer_size)
    {
        host[iteration_count] = *start;
        iteration_count++;
        start++;
    }
    printf("\nHOST ATTEMPT: %s\n", host);

    // return host;
    return NULL;
}

char *ParseHost(char *buffer, size_t buffer_size)
{

    printf("\n[INFO]: entering ParseHost\n");
    if (buffer == NULL || buffer_size == 0)
    {
        printf("\n[ERROR]: buffer input for ParseHost is NULL :O\n");
        return NULL;
    }

    char *temp_ptr = buffer;
    char *start = buffer;
    char *end = buffer;
    size_t size = 0;
    int colon_count = 0;

    while (size < buffer_size && *temp_ptr != '\0' && *temp_ptr != '\n')
    {
        if (*temp_ptr == ':')
        {
            colon_count++;
        }
        if (colon_count == 1)
        {
            start = temp_ptr;
        }
        if (colon_count == 2)
        {
            end = temp_ptr;
            printf("\n[INFO]: found 2 colons!\n");
            break;
        }
        temp_ptr++;
        size++;
    }
    temp_ptr = NULL;
    size_t distance = end - start;
    distance++; // for '\0'
    if (*start != ':' && *end != ':')
    {
        printf("\n[FATAL]: cannot find colons\n");
        return NULL;
    }
    if (*(start + 1) == ' ')
    {
        start++;
    }

    char *host = substrBetweenColons(start, end, distance);

    if (host == NULL)
    {
        return NULL;
    }
    // printf("\nDISTANCE IS: %zd\n", distance);
    return NULL;
}
