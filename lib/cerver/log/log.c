#include "log.h"
#include <stdbool.h>
#include <stdlib.h>

void Log(enum LogLevels level)
{
    bool print = true;
    char *code = NULL;
    switch (level)
    {
    case FATAL:
    case ERROR:
    case WARN:
    case DEBUG:
    case INFO:
    case TRACE:
    default:
        break;
    }
}
