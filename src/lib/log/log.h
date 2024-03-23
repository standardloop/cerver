#ifndef LOG_H
#define LOG_H

enum LogLevels
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};
void Log(int level);
#endif