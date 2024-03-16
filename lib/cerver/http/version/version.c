#include "./version.h"

float ParseHttpVersion(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        return 0.0;
    }
    return 1.1;
}
