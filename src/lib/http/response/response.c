#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // write() close()
#include <time.h>
#include <standardloop/logger.h>
#include <standardloop/json.h>
#include <standardloop/util.h>

#include "./response.h"

#define DATE_BUFFER_SIZE 100

char *generateResponseDate();
HashMap *generateDefaultResponseHeaders();
static void copyStringCanary(char *, char *, u_int64_t);

static void copyStringCanary(char *des, char *src, u_int64_t des_offset)
{
    char *des_iterator = des + des_offset;

    while (src != NULL && *src != NULL_CHAR)
    {
        *des_iterator++ = *src++;
    }
}

char *generateResponseDate()
{
    char *date_string = malloc(sizeof(char) * DATE_BUFFER_SIZE);
    if (date_string == NULL)
    {
        return NULL;
    }
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(date_string, sizeof(date_string), "%a, %d %b %Y %H:%M:%S GMT", &tm);

    return date_string;
}

HashMap *generateDefaultResponseHeaders()
{
    HashMap *response_headers = DefaultHashMapInit();

    // Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
    char *date_for_header = generateResponseDate();
    JSONValue *date_for_header_obj = JSONValueInit(STRING_t, date_for_header, QuickAllocatedString("Date"));
    HashMapInsert(response_headers, date_for_header_obj);

    char *server_header = QuickAllocatedString("cerver");
    JSONValue *server_header_obj = JSONValueInit(STRING_t, server_header, QuickAllocatedString("Server"));
    HashMapInsert(response_headers, server_header_obj);

    return response_headers;
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

    char *status_code_phrase = HttpStatusCodeToString(resp->response_code, true);
    if (status_code_phrase == NULL)
    {
        free(resp_as_string);
        return NULL;
    }

    resp_as_string_size += strlen(status_code_phrase);
    resp_as_string = realloc(resp_as_string, resp_as_string_size);

    copyStringCanary(resp_as_string, status_code_phrase, chars_written);

    printf("[JOSH]: %s\n", resp_as_string);

    // <VERSION> <STATUS_CODE> <STATUS_STRING>\n<HEADERS>\n\n<BODY>
    // "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 6\n\nHello!";
    // add HTTP/<VERSION>

    return resp_as_string;
}
