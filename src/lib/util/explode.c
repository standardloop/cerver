#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./util.h"

StringArr *EveryoneExplodeNow(char *input_str, char delim)
{
    if (input_str == NULL || delim == NULL_CHAR)
    {
        return NULL;
    }
    StringArr *string_arr = malloc(sizeof(StringArr));
    if (string_arr == NULL)
    {
        printf("[ERROR]: couldn't allocate memory for StringArr\n");
        return NULL;
    }

    int delim_count = 0;
    int len = strlen(input_str);
    for (int i = 0; i < len; i++)
    {
        if (input_str[i] == delim)
        {
            delim_count++;
        }
    }

    string_arr->num_strings = delim_count + 1;
    string_arr->strings = malloc(sizeof(char *) * string_arr->num_strings);
    if (string_arr->strings == NULL)
    {
        free(string_arr);
        printf("[ERROR]: couldn't allocate memory for strings in side StringArr\n");
        return NULL;
    }
    char *start = input_str;
    char *end = NULL;
    int entry_count = 0;
    while (entry_count < string_arr->num_strings)
    {
        if (*input_str == delim || *input_str == NULL_CHAR)
        {
            end = input_str;
            size_t entry_len = end - start;
            char *entry = malloc(sizeof(char) * entry_len);
            if (entry == NULL)
            {
                printf("[FATAL]: cannot allocate mem for entry string\n");
                exit(1);
            }
            char *entry_iterator = entry;
            while (start != end)
            {
                *entry_iterator = *start;
                entry_iterator++;
                start++;
            }
            *entry_iterator = NULL_CHAR;
            string_arr->strings[entry_count] = entry;
            start = input_str + 1;
            entry_count++;
        }
        if (*input_str != NULL_CHAR)
        {
            input_str++;
        }
    }
    return string_arr;
}

void FreeStringArr(StringArr *string_arr)
{
    if (string_arr == NULL || string_arr->num_strings == 0)
    {
        printf("[DEBUG]: nothing to free\n");
        return;
    }
    int entry_count = 0;
    while (entry_count < string_arr->num_strings)
    {
        if (string_arr->strings[entry_count] != NULL)
        {
            free(string_arr->strings[entry_count]);
        }

        entry_count++;
    }
    if (string_arr->strings != NULL)
    {
        free(string_arr->strings);
    }
    free(string_arr);
    return;
}

void PrintStringArr(StringArr *string_arr)
{
    if (string_arr == NULL || string_arr->num_strings == 0)
    {
        return;
    }
    printf("[");
    for (int i = 0; i < string_arr->num_strings; i++)
    {
        if (i == string_arr->num_strings - 1)
        {
            printf("\"%s\"]", string_arr->strings[i]);
        }
        else
        {
            printf("\"%s\", ", string_arr->strings[i]);
        }
    }
    printf("\n");
}

void PrintStringArrDebug(StringArr *string_arr)
{
    if (string_arr == NULL || string_arr->num_strings == 0)
    {
        return;
    }
    for (int i = 0; i < string_arr->num_strings; i++)
    {
        int len = strlen(string_arr->strings[i]);
        for (int j = 0; j <= len; j++)
        {
            printf("%d ", string_arr->strings[i][j]);
        }
        printf("\n");
    }
}
