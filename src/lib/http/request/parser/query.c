#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./parser.h"
#include "./../../../util/util.h"
#include "./../../../logger.h"
#include "./../../../map/map.h"

void copyString(char *, char *, size_t);

// FIXME: move to util and finalize this one
void copyString(char *src, char *des, size_t len)
{
    char *src_it = src;
    char *dest_it = des;
    size_t size = 0;
    while (size < len)
    {
        *dest_it = *src_it;
        dest_it++;
        src_it++;
        size++;
    }
}

Map *ParseQuery(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size <= 0)
    {
        (void)Log(WARN, "invalid inputs to ParserQuery\n");
        return NULL;
    }
    // (void)PrintBuffer(buffer, buffer_size, false);
    // (void)Log(FATAL, "");
    Map *query_map = InitMap(MAX_QUERY_ENTRIES);
    if (query_map == NULL)
    {
        (void)Log(WARN, "[5XX] unable to allocate memory to create query map\n");
        return NULL;
    }

    size_t temp_size = 0;
    char *buffer_iterator = buffer;

    char *key_start = buffer;
    char *value_start = NULL;

    char *query_key = NULL;
    char *query_value = NULL;

    while (buffer_iterator != NULL && *buffer_iterator != CARRIAGE_RETURN_CHAR && temp_size < buffer_size)
    {
        // printf("\n%c\n", *buffer_iterator);
        if (*buffer_iterator == EQUAL_CHAR)
        {
            value_start = buffer_iterator + 1;
            size_t key_size = (value_start - key_start);
            // printf("\n[JOSH]: %d\n", (int)key_size);
            // (void)Log(FATAL, "");
            query_key = malloc(sizeof(char) * key_size);
            if (query_key == NULL)
            {
                (void)Log(WARN, "cannot allocate memory to parse a query key\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_key + key_size) = NULL_CHAR;
            (void)copyString(key_start, query_key, key_size - 1);
            // printf("\n[JOSH]: %s\n", query_key);
        }
        else if (*buffer_iterator == AND_CHAR)
        {
            key_start = buffer_iterator + 1;
            size_t value_size = (value_start - key_start);
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                (void)Log(WARN, "cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            (void)copyString(value_start, query_value, value_size - 1);
            // printf("\n[JOSH]: %s\n", query_value);
        }
        else if (*buffer_iterator == NULL_CHAR)
        {
            size_t value_size = (buffer_iterator - value_start) + 1;
            printf("\n[JOSH]: %d\n", (int)value_size);
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                (void)Log(WARN, "cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            (void)copyString(value_start, query_value, value_size - 1);
            // printf("\n[JOSH]: %s\n", query_value);
        }

        if (query_key != NULL && query_value != NULL && query_key != query_value)
        {
            (void)Log(TRACE, "adding a query entry to the map!\n");
            (void)MapAdd(query_map, query_key, query_value);
        }
        buffer_iterator++;
        temp_size++;
    }
    (void)PrintMap(query_map);
    return query_map;
}
