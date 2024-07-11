#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./parser.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>
#include <standardloop/json.h>

// id=3&bar=1
HashMap *ParseQuery(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size <= 0)
    {
        Log(WARN, "invalid inputs to ParserQuery\n");
        return NULL;
    }
    // PrintBuffer(buffer, buffer_size, false);
    // Log(FATAL, "");
    HashMap *query_map = DefaultHashMapInit();
    if (query_map == NULL)
    {
        Log(WARN, "[5XX] unable to allocate memory to create query map\n");
        return NULL;
    }

    size_t temp_size = 0;
    char *buffer_iterator = buffer;

    char *key_start = buffer;
    char *value_start = buffer;

    char *query_key = NULL;
    char *query_value = NULL;

    u_int32_t query_entry_count = 0;
    while (buffer_iterator != NULL && temp_size < buffer_size)
    {
        if (*buffer_iterator == EQUAL_CHAR)
        {
            value_start = buffer_iterator + 1;
            size_t key_size = (value_start - key_start);
            query_key = malloc(sizeof(char) * key_size);
            if (query_key == NULL)
            {
                Log(WARN, "cannot allocate memory to parse a query key\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_key + key_size) = NULL_CHAR;
            CopyStringServer(key_start, query_key, key_size - 1, 0);
        }
        else if (*buffer_iterator == AND_CHAR)
        {
            key_start = buffer_iterator + 1;                         // next key start
            size_t value_size = (buffer_iterator - value_start) + 1; // +1 for NULL char
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                Log(WARN, "& char cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            CopyStringServer(value_start, query_value, value_size - 1, 0); // -1 because nullchar is accounted for
        }
        // FIXME: allowed characters in query string?
        else if (*buffer_iterator == NULL_CHAR || *buffer_iterator == CARRIAGE_CHAR || *buffer_iterator == SPACE_CHAR || *buffer_iterator == CARRIAGE_CHAR)
        {
            size_t value_size = (buffer_iterator - value_start) + 1;
            query_value = malloc(sizeof(char) * value_size);
            if (query_value == NULL)
            {
                Log(WARN, "end of line cannot allocate memory to parse a query value\n");
                // FIXME memory leak
                return query_map;
            }
            *(query_value + value_size) = NULL_CHAR;
            CopyStringServer(value_start, query_value, value_size - 1, 0);
        }

        if (query_key != NULL && query_value != NULL && query_key != query_value)
        {
            // Log(TRACE, "adding a query entry to the map!\n");
            (void)HashMapInsert(query_map, JSONValueInit(STRING_t, query_value, query_key)); // FIXME error checking
            query_entry_count++;
            query_key = NULL;
            query_value = NULL;
        }
        buffer_iterator++;
        temp_size++;
    }
    if (query_entry_count != query_map->size + query_map->collision_count)
    {
        // FIXME
        Log(WARN, "Query Map Size is not equal to our expected count\n");
    }

    // PrintMap(query_map);
    return query_map;
}
