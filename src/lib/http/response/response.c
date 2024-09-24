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
    return NULL;
}
