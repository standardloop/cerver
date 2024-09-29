#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // write() close()
#include <time.h>
#include <standardloop/logger.h>
#include <standardloop/json.h>
#include <standardloop/util.h>

#include "./response.h"

char *generateResponseDate();
HashMap *generateDefaultResponseHeaders();
char *headersToString(HashMap *);

#define DATE_BUFFER_SIZE 100
char *generateResponseDate()
{
    char *date_string = malloc(sizeof(char) * DATE_BUFFER_SIZE);
    if (date_string == NULL)
    {
        return NULL;
    }
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(date_string, sizeof(char) * DATE_BUFFER_SIZE, "%a, %d %b %Y %H:%M:%S GMT", &tm);

    return date_string;
}

HashMap *generateDefaultResponseHeaders()
{
    HashMap *response_headers = DefaultHashMapInit();

    // Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
    JSONValue *date_for_header_obj = JSONValueInit(STRING_t, generateResponseDate(), QuickAllocatedString("Date"));
    HashMapInsert(response_headers, date_for_header_obj);

    JSONValue *server_header_obj = JSONValueInit(STRING_t, QuickAllocatedString("cerver"), QuickAllocatedString("Server"));
    HashMapInsert(response_headers, server_header_obj);

    // Content-Type: text/plain FIXME: HARDCODED
    JSONValue *content_header_obj = JSONValueInit(STRING_t, QuickAllocatedString("text/plain"), QuickAllocatedString("Server"));
    HashMapInsert(response_headers, content_header_obj);

    return response_headers;
}

char *headersToString(HashMap *headers)
{
    if (headers == NULL)
    {
        return NULL;
    }
    size_t headers_as_string_size = 1; // \0
    char *headers_as_string = malloc(sizeof(char) * headers_as_string_size);
    headers_as_string[0] = NULL_CHAR;

    size_t chars_written = 0;

    for (u_int64_t i = 0; i < headers->capacity; i++)
    {
        JSONValue *headers_entry = headers->entries[i];
        bool needs_newline = true;

        while (headers_entry != NULL)
        {
            char *entry_key = headers_entry->key;
            size_t duplicated_key_size = strlen(entry_key);
            char *duplicated_key = QuickAllocatedString(entry_key);
            duplicated_key_size += 0;

            char *entry_value = headers_entry->value;
            size_t entry_value_len = strlen(entry_value);

            headers_as_string_size += duplicated_key_size;
            headers_as_string_size += 2; // ": "
            headers_as_string_size += entry_value_len;
            headers_as_string_size += needs_newline;

            headers_as_string = realloc(headers_as_string, headers_as_string_size);

            CopyStringCanary(headers_as_string, duplicated_key, chars_written);
            chars_written += duplicated_key_size;
            CopyStringCanary(headers_as_string, ": ", chars_written);
            chars_written += 2;
            CopyStringCanary(headers_as_string, entry_value, chars_written);
            chars_written += entry_value_len;
            if (needs_newline)
            {
                CopyStringCanary(headers_as_string, "\n", chars_written);
                chars_written++;
            }
            free(duplicated_key);

            needs_newline = true;
            headers_entry = headers_entry->next;
        }
    }
    headers_as_string[headers_as_string_size - 1] = NULL_CHAR;
    return headers_as_string;
}

void FreeHttpResponse(HttpResponse *response)
{
    if (response != NULL)
    {
        if (response->headers != NULL)
        {
            FreeHashMap(response->headers);
        }

        free(response);
    }
}

HttpResponse *CreateHttpResponse()
{
    HttpResponse *response = malloc(sizeof(HttpResponse));
    if (response == NULL)
    {
        return NULL;
    }
    response->response_code = 0;
    response->version = NULL;
    response->body = NULL;
    response->headers = generateDefaultResponseHeaders();
    if (response->headers == NULL)
    {
        // FIXME
        Log(ERROR, "couldn't allocate memory for response headers");
    }

    return response;
}

char *HttpResponseToString(HttpResponse *resp)
{
    if (resp == NULL)
    {
        return NULL;
    }
    size_t resp_as_string_size = 4 + 1 + 3 + 1 + 3 + 1 + 1; // HTTP/1.1 XXX \0
    char *resp_as_string = malloc(sizeof(char) * resp_as_string_size);
    if (resp_as_string == NULL)
    {
        return NULL;
    }
    size_t chars_written = 0;
    resp_as_string[chars_written] = 'H';
    chars_written++;
    resp_as_string[chars_written] = 'T';
    chars_written++;
    resp_as_string[chars_written] = 'T';
    chars_written++;
    resp_as_string[chars_written] = 'P';
    chars_written++;
    resp_as_string[chars_written] = FORWARDLASH_CHAR;
    chars_written++;
    resp_as_string[chars_written] = resp->version[0];
    chars_written++;
    resp_as_string[chars_written] = resp->version[1];
    chars_written++;
    resp_as_string[chars_written] = resp->version[2];
    chars_written++;
    resp_as_string[chars_written] = SPACE_CHAR;
    chars_written++;

    char *status_code_as_str = HttpStatusCodeToString(resp->response_code, false);
    if (status_code_as_str == NULL)
    {
        free(resp_as_string);
        return NULL;
    }
    for (size_t i = 0; i < 3; i++)
    {
        resp_as_string[chars_written] = status_code_as_str[i];
        chars_written++;
    }
    resp_as_string[chars_written] = SPACE_CHAR;
    chars_written++;

    // Status code phrase
    char *status_code_phrase = HttpStatusCodeToString(resp->response_code, true);
    if (status_code_phrase == NULL)
    {
        free(resp_as_string);
        return NULL;
    }
    size_t status_code_phrase_size = strlen(status_code_phrase) + 1; // +1 for \n
    resp_as_string_size += status_code_phrase_size;
    resp_as_string = realloc(resp_as_string, resp_as_string_size);
    CopyStringCanary(resp_as_string, status_code_phrase, chars_written);
    resp_as_string[resp_as_string_size] = NEWLINE_CHAR;
    chars_written += resp_as_string_size;

    printf("[JOSH1]:\n");
    printf("%s", resp_as_string);
    printf("%d\n", (int)resp_as_string_size);
    fflush(stdout);

    // Headers
    char *headers_as_string = headersToString(resp->headers);
    size_t headers_as_string_size = strlen(headers_as_string) + 1; //  +1 for \n
    resp_as_string_size += headers_as_string_size;
    resp_as_string = realloc(resp_as_string, resp_as_string_size);
    CopyStringCanary(resp_as_string, headers_as_string, chars_written);
    chars_written += headers_as_string_size;
    resp_as_string[resp_as_string_size] = NEWLINE_CHAR;

    printf("[JOSH]:\n");
    printf("%s", resp_as_string);
    fflush(stdout);

    exit(23);

    // <VERSION> <STATUS_CODE> <STATUS_STRING>\n<HEADERS>\n\n<BODY>
    // "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";

    return resp_as_string;
}
