#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>

#include "./util.h"

bool RegexBoolMatch(char *pattern, char *string)
{
    regex_t rgT;
    regmatch_t match;
    bool is_match = false;
    regcomp(&rgT, pattern, REG_EXTENDED);
    if ((regexec(&rgT, string, 1, &match, 0)) == 0)
    {
        is_match = true;
    }
    regfree(&rgT);
    return is_match;
}

char *RegexReturnMatch(char *pattern, char *string, int *begin, int *end)
{
    int i, w = 0, len;
    char *word = NULL;
    regex_t rgT;
    regmatch_t match;
    regcomp(&rgT, pattern, REG_EXTENDED);
    if ((regexec(&rgT, string, 1, &match, 0)) == 0)
    {
        *begin = (int)match.rm_so;
        *end = (int)match.rm_eo;
        len = *end - *begin;
        word = (char *)malloc(sizeof(char) * (len + 1));
        for (i = *begin; i < *end; i++)
        {
            word[w] = string[i];
            w++;
        }
        word[w] = '\0';
    }
    regfree(&rgT);
    return word;
}
