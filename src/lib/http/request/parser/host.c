#include <stdbool.h>

#include "./host.h"
#include "./../../../util/util.h"
#include "./../../../logger.h"

char *ParseHost(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        (void)Log(WARN, "\n[4XX]: buffer input for ParseHost is NULL \n");
        return NULL;
    }
    return NULL;
}
