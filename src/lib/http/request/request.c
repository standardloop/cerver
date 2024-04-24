#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <pthread.h>
#include <stdbool.h>

#include "./request.h"
#include "./../response/codes.h"

#include "./parser/parser.h"

#include "../../util/util.h"
#include "../../logger.h"

HttpRequest *ParseHttpRequest(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        (void)Log(ERROR, "[4XX]: buffer is NULL for ParseHttpRequest or buffer_size is 0\n");
        return NULL;
    }
    HttpRequest *request = (HttpRequest *)malloc(sizeof(HttpRequest));
    if (request == NULL)
    {
        (void)Log(ERROR, "[5XX]: buffer is NULL for malloc in ParseHttpRequest\n");
        return NULL;
    }
    request->early_resp_code = 0;
    /*
    *********************************************************************************
        METHOD
    *********************************************************************************
    */

    char *buffer_start = buffer;
    char *space_pointer = strchr(buffer, SPACE_CHAR);
    if (*space_pointer != SPACE_CHAR)
    {
        Log(WARN, "[4XX]: space_point is not a space!\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    size_t suspected_http_method_length = (space_pointer - buffer_start);

    request->method = ParseRequestMethod(buffer_start, suspected_http_method_length);
    if (request->method == HttpFAKE)
    {
        Log(WARN, "[4XX]: Couldn't parse HTTP Request method (HttpFAKE)\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    /*
    *********************************************************************************
        PATH + QUERY
        HEAD /asdsadsd/ash?id=3 HTTP/1.1
    *********************************************************************************
    */
    buffer = buffer_start;
    char *second_space_pointer = strchr((space_pointer + 1), SPACE_CHAR); // +1 because pointer is on space
    if ((*second_space_pointer != SPACE_CHAR) && (*space_pointer != SPACE_CHAR))
    {
        (void)Log(WARN, "error finding appropriate amount of spaces for path+query");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    size_t suspected_path_length;
    char *question_mark_char = strchr((space_pointer + 1), QUESTION_CHAR);

    if (question_mark_char == NULL)
    {
        //(void)Log(INFO, "the request does not contain a query\n");
        request->query = NULL;
        suspected_path_length = (second_space_pointer - space_pointer);
    }
    else
    {
        (void)Log(INFO, "the request contains a query!\n");
        size_t suspected_query_length = second_space_pointer - question_mark_char;
        request->query = ParseQuery(question_mark_char + 1, suspected_query_length);
        if (request->query == NULL)
        {
            (void)Log(ERROR, "request->query is NULL");
            request->early_resp_code = HttpBadGateway; // FIXME
            return request;
        }
        suspected_path_length = ((question_mark_char - 1) - space_pointer);
    }
    request->path = ParseRequestPath(space_pointer + 1, suspected_path_length);
    if (request->path == NULL)
    {
        Log(WARN, "[4XX]: Couldn't parser HTTP Request path/query\n");
        request->early_resp_code = HttpBadGateway;
        return request;
    }
    /*
    *********************************************************************************
        VERSION
    *********************************************************************************
    */
    // HTTP/1.1\r\n
    buffer = buffer_start;
    char *carriage_return_ptr = strchr(second_space_pointer, CARRIAGE_RETURN_CHAR);
    if (*carriage_return_ptr != CARRIAGE_RETURN_CHAR)
    {
        (void)Log(WARN, "[4XX]: cannot find carriage return\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    if (*second_space_pointer != SPACE_CHAR)
    {
        (void)Log(WARN, "[4XX]: found carriage return but second space error\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    size_t expected_verion_length = carriage_return_ptr - (second_space_pointer + 1);
    request->version = ParseHttpVersion((second_space_pointer + 1), expected_verion_length);
    if (request->version == NULL)
    {
        (void)Log(WARN, "[4XX]: couldn't parser http version\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    /*
    *********************************************************************************
        HOST
    *********************************************************************************
    */

    buffer = buffer_start;
    char *newline_ptr = strchr(second_space_pointer, NEWLINE_CHAR);
    if (*newline_ptr != NEWLINE_CHAR)
    {
        (void)Log(ERROR, "[4XX]: cannot find newline\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    if (*(newline_ptr + 1) != 'H')
    {
        (void)Log(ERROR, "[4XX]: H not present after newline\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    if (*(newline_ptr + 6) != SPACE_CHAR)
    {
        (void)Log(ERROR, "[4XX]: cannot find space after Host:\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    char *suspected_host_start = (newline_ptr + 7); // skip over "Host: "
    char *colon_ptr = strchr(suspected_host_start, COLON_CHAR);
    if (*colon_ptr != COLON_CHAR)
    {
        (void)Log(WARN, "[4XX]: cannot find colon so port wasn't included?\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    size_t expected_host_length = (colon_ptr - suspected_host_start);
    request->host = ParseHost(suspected_host_start, expected_host_length);
    if (request->host == NULL)
    {
        (void)Log(ERROR, "[4XX]: cannot parse host from request\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    /*
    *********************************************************************************
        PORT
    *********************************************************************************
    */

    buffer = buffer_start;
    if (*newline_ptr != NEWLINE_CHAR || *colon_ptr != COLON_CHAR)
    {
        (void)Log(ERROR, "[4XX]: cannot parse host from request\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    char *second_carriage_return_ptr = strchr(colon_ptr, CARRIAGE_RETURN_CHAR);
    if (*second_carriage_return_ptr != CARRIAGE_RETURN_CHAR)
    {
        (void)Log(ERROR, "[4XX]: couldn't find 2nd carriage return\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }
    size_t expected_port_length = second_carriage_return_ptr - (++colon_ptr);
    request->port = ParsePort(colon_ptr, expected_port_length);
    if (request->port == ERROR_PORT)
    {
        (void)Log(ERROR, "[4XX]: couldn't parse port");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    /*
    *********************************************************************************
        REMAINING? HEADERS
    *********************************************************************************
    */

    buffer = buffer_start;
    newline_ptr = strchr(newline_ptr + 1, NEWLINE_CHAR);

    char *headers_ptr = newline_ptr + 1;
    request->headers = ParseHeaders(headers_ptr, buffer_size);

    if (request->headers == NULL)
    {
        (void)Log(ERROR, "[4XX]: couldn't parse headers\n");
        request->early_resp_code = HttpBadRequest;
        return request;
    }

    PrintMap(request->headers);
    (void)Log(FATAL, "");

    return request;
}

void FreeHttpRequest(HttpRequest *request)
{
    if (request == NULL)
    {
        return;
    }
    if (request->path != NULL)
    {
        free(request->path);
    }
    if (request->query != NULL)
    {
        free(request->query);
    }
    if (request->version != NULL)
    {
        free(request->version);
    }
    if (request->host != NULL)
    {
        free(request->host);
    }
    if (request->headers != NULL)
    {
        FreeMap(request->headers);
    }
    free(request);
}

// FIXME user logger here
void PrintHttpRequest(HttpRequest *request)
{
    if (request->method != HttpFAKE)
    {
        printf("[DEBUG][HTTPMETHOD]: %s\n", HttpMethodToStr(request->method));
    }
    if (request->host != NULL)
    {
        printf("[DEBUG][HTTPHOST]: %s\n", request->host);
    }
    if (request->port != 0)
    {
        printf("[DEBUG][HTTPPORT]: %d\n", request->port);
    }
}
