#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NULL_CHAR '\0'
#define SPACE_CHAR ' '
#define NEWLINE_CHAR '\n'
#define SLASH_CHAR '/'
#define CARRIAGE_RETURN_CHAR '\r'
#define COLON_CHAR ':'
#define QUESTION_CHAR '?'
#define EQUAL_CHAR '='
#define AND_CHAR '&'

#define ALWAYS 1

void PrintBuffer(char *, size_t, bool);
char *StrToUpper(char *);
char *StrToLower(char *);
const char *GetEnv(char *, char *);

#endif
