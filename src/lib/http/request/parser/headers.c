#include <stdlib.h>
#include <stdio.h>

#include "./parser.h"
#include "./../../../logger.h"

Map *ParseHeaders(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0 || buffer_size > MAX_PORT_LENGTH)
    {
        (void)Log(ERROR, "[ERROR][4XX]: invalid buffer for parsing port");
        return NULL;
    }
    return NULL;
}
