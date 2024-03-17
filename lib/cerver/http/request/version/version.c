#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "./version.h"
#include "../../../util/util.h"

float substrToFloat(char *);

float substrToFloat(char *version_string)
{
    // printf("\n%s\n", version_string);
    if (version_string == NULL)
    {
        return ERROR_FLOAT;
    }
    float final_answer = roundf(atof(version_string) * 1000) / 1000;
    free(version_string);  // :O
    return final_answer;
}
float ParseHttpVersion(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        return ERROR_FLOAT;
    }
    char *temp_ptr = buffer;
    size_t char_count = 0;

    int slash_count = 0;
    // GET / HTTP/1.1

    // FIXME: disgusting code
    char *version_string = NULL;
    while (char_count < buffer_size && *temp_ptr != '\0' && *temp_ptr != '\n')
    {
        // printf("%c", *temp_ptr);
        if (*temp_ptr == '/')
        {
            slash_count++;
        }

        temp_ptr++;
        char_count++;
        if (slash_count == 2)
        {
            break;
        }
    }

    if (slash_count == 2)
    {
        // printf("\nSlash Count is 2 and *temp_ptr is: %c\n", *temp_ptr);
        // temp_ptr++;
        // printf("\nSlash Count is 2 and *temp_ptr+1 is: %c\n", *temp_ptr);
        // temp_ptr++;
        // printf("\nSlash Count is 2 and *temp_ptr+2 is: %c\n", *temp_ptr);
        // 1.1
        size_t substr_count = 4 * sizeof(char);
        version_string = malloc(sizeof(char) * substr_count);
        if (version_string == NULL)
        {
            return ERROR_FLOAT;
        }
        for (size_t i = 0; i < (substr_count - 1); i++)
        {
            version_string[i] = *temp_ptr;
            temp_ptr++;
        }

        version_string[3] = '\0';
        // size_t i = 0;
        // while (i < substr_count - 1 && char_count < buffer_size && *temp_ptr != '\0' && *temp_ptr != '\n')
        // {
        //     version_string[i] = *temp_ptr;
        //     if (i == 1 && *temp_ptr != '.')
        //     {
        //         free(version_string);
        //         return ERROR_FLOAT;
        //     }
        //     temp_ptr++;
        //     char_count++;
        // }
        // // printf("\nversion_string: %s\n", version_string);
    }
    else
    {
        return ERROR_FLOAT;
    }
    temp_ptr = NULL;
    if (version_string == NULL)
    {
        return ERROR_FLOAT;
    }
    return substrToFloat(version_string);
    // return 1.1;
}
