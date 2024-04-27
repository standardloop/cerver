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
// id=3&bar=1
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
    char *value_start = buffer;

    char *query_key = NULL;
    char *query_value = NULL;

    int query_entry_count = 0;
    while (buffer_iterator != NULL && temp_size < buffer_size)
    {
        if (*buffer_iterator == EQUAL_CHAR)
        {
            value_start = buffer_iterator + 1;
            size_t key_size = (value_start - key_start);
            // printf("\n[JOSH]: %d\n", (int)key_size);
            //  (void)Log(FATAL, "");
            query_key = malloc(sizeof(char) * key_size);
            if (query_key == NULL)
            {
                (void)Log(WARN, "cannot allocate memory to parse a query key\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_key + key_size) = NULL_CHAR;
            (void)copyString(key_start, query_key, key_size - 1);
        }
        else if (*buffer_iterator == AND_CHAR)
        {
            key_start = buffer_iterator + 1;                         // next key start
            size_t value_size = (buffer_iterator - value_start) + 1; // +1 for NULL char
            printf("\n[value_size]: %d\n", (int)value_size);
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                (void)Log(WARN, "& char cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            (void)copyString(value_start, query_value, value_size - 1); // -1 because nullchar is accounted for
        }
        // FIXME: allowed characters in query string?
        else if (*buffer_iterator == NULL_CHAR || *buffer_iterator == CARRIAGE_RETURN_CHAR || *buffer_iterator == SPACE_CHAR || *buffer_iterator == CARRIAGE_RETURN_CHAR)
        {
            size_t value_size = (buffer_iterator - value_start) + 1;
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                (void)Log(WARN, "end of line cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            (void)copyString(value_start, query_value, value_size - 1);
        }

        if (query_key != NULL && query_value != NULL && query_key != query_value)
        {
            //(void)Log(TRACE, "adding a query entry to the map!\n");
            (void)MapAdd(query_map, query_key, query_value); // FIXME error checking
            query_entry_count++;
            query_key = NULL;
            query_value = NULL;
        }
        buffer_iterator++;
        temp_size++;
    }
    if (query_entry_count != query_map->count)
    {
        // FIXME
        (void)Log(WARN, "Query Map Size is not equal to our expected count\n");
    }

    (void)PrintMap(query_map);
    return query_map;
}
