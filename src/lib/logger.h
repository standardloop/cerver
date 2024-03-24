#ifndef LOGGER_H
#define LOGGER_H
#include <stdbool.h>

enum LogLevel
{
    TRACE = 5,
    DEBUG = 4,
    INFO = 3,
    WARN = 2,
    ERROR = 1,
    FATAL = 0,
};

void Log(enum LogLevel, char *);
void SetLogLevel(enum LogLevel);

#endif
