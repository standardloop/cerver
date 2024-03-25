#include <stdbool.h>

#include "./port.h"
#include "./../../../util/util.h"
#include "./../../../logger.h"

int ParsePort(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        (void)Log(ERROR, "[ERROR][4XX]: invalid buffer for parsing port");
        return ERROR_PORT;
    }

    return ERROR_PORT;
}
