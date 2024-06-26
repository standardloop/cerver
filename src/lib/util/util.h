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
#define DOLLAR_CHAR '$'
#define CARROT_CHAR '^'

#define ALWAYS 1

void PrintBuffer(char *, size_t, bool);

// string
char *StrToUpper(char *);
char *StrToLower(char *);
void CopyString(char *, char *, size_t, size_t);

// env
const char *GetEnv(char *, char *);

// regex
bool RegexBoolMatch(char *, char *);
char *RegexReturnMatch(char *, char *, int *, int *);

// explode
typedef struct
{
    char **strings;
    int num_strings;
} StringArr;

StringArr *EveryoneExplodeNow(char *, char);
void FreeStringArr(StringArr *);
void PrintStringArr(StringArr *);
void PrintStringArrDebug(StringArr *);

#endif
