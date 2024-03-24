#include "logger.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static enum LogLevel log_level = FATAL;

void SetLogLevel(enum LogLevel level)
{
    log_level = level;
}

enum LogLevel GetLogLevel()
{
    return log_level;
}

void Log(enum LogLevel level, char *message)
{
    // FIXME: Log configuration
    // char buff[1024];
    // memset(buff, '\0', sizeof(buff));
    // setvbuf(stdout, buff, _IOFBF, 1024);

    // FIXME: have time as well
    // (int)time(NULL);
    // FIXME: allow configuration such as JSON logs etc
    if (level > log_level)
    {
        return;
    }
    switch (level)
    {
    case FATAL:
        printf("\n[FATAL]: %s", message);
        exit(EXIT_FAILURE);
    case ERROR:
        printf("\n[ERROR]: %s", message);
    case WARN:
        printf("\n[WARN]: %s", message);
    case DEBUG:
        printf("\n[DEBUG]: %s", message);
    case INFO:
        printf("\n[INFO]: %s", message);
    case TRACE:
        printf("\n[TRACE]: %s", message);
    }
}
