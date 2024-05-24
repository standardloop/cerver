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
    if (regcomp(&rgT, pattern, REG_EXTENDED) == 0)
    {
        if ((regexec(&rgT, string, 1, &match, 0)) == 0)
        {
            is_match = true;
        }
    }
    (void)regfree(&rgT);
    return is_match;
}

char *RegexReturnMatch(char *pattern, char *string, int *begin, int *end)
{
    char *word = NULL;
    regex_t rgT;
    regmatch_t match;
    regcomp(&rgT, pattern, REG_EXTENDED);
    if ((regexec(&rgT, string, 1, &match, 0)) == 0)
    {
        *begin = (int)match.rm_so;
        *end = (int)match.rm_eo;
        int len = *end - *begin;
        word = (char *)malloc(sizeof(char) * (len + 1));
        int w = 0;
        for (int i = *begin; i < *end; i++)
        {
            word[w] = string[i];
            w++;
        }
        word[w] = '\0';
    }
    regfree(&rgT);
    return word;
}
