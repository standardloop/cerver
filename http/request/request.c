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

#include <standardloop/util.h>
#include <standardloop/logger.h>
#include <standardloop/json.h>

static char *locateQueryStart(char *, size_t);
static HttpRequest *createBlankHttpRequest();

static char *locateQueryStart(char *buffer, size_t size)
{
    size_t char_count = 0;
    char *buffer_iterator = buffer;
    while (char_count < size && buffer_iterator != NULL &&
           *buffer_iterator != CARRIAGE_CHAR &&
           *buffer_iterator != NEWLINE_CHAR &&
           *buffer_iterator != NULL_CHAR)
    {
        if (*buffer_iterator == QUESTION_CHAR)
        {
            return buffer_iterator;
        }
        buffer_iterator++;
        char_count++;
    }
    return NULL;
}

static HttpRequest *createBlankHttpRequest()
{
    HttpRequest *request = malloc(sizeof(HttpRequest));
    if (request == NULL)
    {
        return NULL;
    }
    request->client_socket = -1;
    request->method = HttpGET;
    request->path = NULL;
    request->query_params = NULL;
    request->version = NULL;
    request->host = NULL;
    request->port = -1;
    request->bail_resp_code = HttpBadGateway;
    request->headers = NULL;
    request->body = NULL;
    request->path_params = NULL;

    return request;
}

HttpRequest *CreateParsedHttpRequest(char *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size == 0)
    {
        Log(ERROR, "[4XX]: buffer is NULL for CreateParsedHttpRequest or buffer_size is 0\n");
        return NULL;
    }
    HttpRequest *request = createBlankHttpRequest();
    if (request == NULL)
    {
        Log(ERROR, "[5XX]: no memory for malloc-ing HttpRequest\n");
        return NULL;
    }
    request->bail_resp_code = 0;

    char *current_line_in_http_request = buffer;
    u_int64_t line_count = 1;
    bool parse_body = false;
    size_t chars_found = 0; // FIXME how to track this
    bool headers_map_created = false;
    bool found_last_line = false;
    while (current_line_in_http_request && chars_found < buffer_size)
    {
        char *next_line_in_http_request = strchr(current_line_in_http_request, '\n');
        if (next_line_in_http_request != NULL)
        {
            *next_line_in_http_request = NULL_CHAR; // temporarily terminate the current line
        }

        chars_found += strlen(current_line_in_http_request) + 1; // FIXME: SLOW

        // Parse Method Path, Query, and Version
        if (line_count == 1 && current_line_in_http_request != NULL)
        {
            // Method
            char *first_space_pointer = strchr(current_line_in_http_request, SPACE_CHAR);
            if (*first_space_pointer != SPACE_CHAR)
            {
                Log(WARN, "[4XX]: space_point is not a space!\n");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            size_t suspected_http_method_length = (first_space_pointer - current_line_in_http_request);
            suspected_http_method_length++; // NULL_CHAR

            request->method = ParseRequestMethod(current_line_in_http_request, suspected_http_method_length);
            if (request->method == HttpFAKE)
            {
                Log(WARN, "[4XX]: Couldn't parse HTTP Request method (HttpFAKE)\n");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            if (request->method == HttpPOST || request->method == HttpPUT)
            {
                parse_body = true;
            }
            // Path and Query
            char *second_space_pointer = strchr((first_space_pointer + 1), SPACE_CHAR); // +1 because pointer is on space
            if ((*second_space_pointer != SPACE_CHAR) || (*first_space_pointer != SPACE_CHAR))
            {
                Log(WARN, "error finding appropriate amount of spaces for path+query");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            size_t suspected_path_length = second_space_pointer - (first_space_pointer + 1);
            suspected_path_length++; // NULL_CHAR
            request->path = ParseRequestPath(first_space_pointer + 1, suspected_path_length);
            if (request->path == NULL)
            {
                Log(WARN, "[4XX]: Couldn't parse HTTP Request path/query\n");
                request->bail_resp_code = HttpBadGateway;
                return request;
            }
            size_t path_length = strlen(request->path);
            if (path_length < suspected_path_length)
            {
                Log(TRACE, "the request contains a query\n");
                char *question_mark_char = locateQueryStart((first_space_pointer + 1 + path_length), suspected_path_length - path_length);
                if (question_mark_char == NULL || *question_mark_char != QUESTION_CHAR)
                {
                    request->query_params = NULL;
                    // Log(FATAL, "query");
                }
                else
                {
                    size_t query_length = suspected_path_length - path_length;
                    request->query_params = ParseQuery(question_mark_char + 1, query_length);
                    if (request->query_params == NULL)
                    {
                        request->bail_resp_code = HttpBadGateway;
                        return request;
                    }
                }
            }
            else
            {
                Log(TRACE, "the request does not contain a query");
            }

            // Version
            char *carriage_return_ptr = strchr(second_space_pointer, CARRIAGE_CHAR);
            if (*carriage_return_ptr != CARRIAGE_CHAR)
            {
                Log(WARN, "[4XX]: cannot find carriage return");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            if (*second_space_pointer != SPACE_CHAR)
            {
                Log(WARN, "[4XX]: found carriage return but second space error");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }

            size_t expected_verion_length = carriage_return_ptr - (second_space_pointer + 1);
            expected_verion_length++; // NULL_CHAR
            request->version = ParseHttpVersion((second_space_pointer + 1), expected_verion_length);
            if (request->version == NULL)
            {
                Log(WARN, "[4XX]: couldn't parser http version");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
        }
        // Host and Port
        // Host is currently required as of HTTP/1.1
        else if (line_count == 2 && current_line_in_http_request != NULL)
        {
            // FIXME maybe fully check for HOST: instead of just checking for H and SPACE_CHAR
            if (*(current_line_in_http_request) != 'H')
            {
                Log(ERROR, "[4XX]: H not present after newline");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            if (*(current_line_in_http_request + 5) != SPACE_CHAR)
            {
                Log(ERROR, "[4XX]: cannot find space after Host:");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            char *suspected_host_start = (current_line_in_http_request + 6); // skip over "Host: "
            char *colon_ptr = strchr(suspected_host_start, COLON_CHAR);
            if (*colon_ptr != COLON_CHAR)
            {
                Log(WARN, "[4XX]: cannot find colon so port wasn't included?");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            size_t expected_host_length = (colon_ptr - suspected_host_start);
            expected_host_length++; // NULL_CHAR
            request->host = ParseHost(suspected_host_start, expected_host_length);
            if (request->host == NULL)
            {
                Log(ERROR, "[4XX]: cannot parse host from request");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }

            // Port
            if (*colon_ptr != COLON_CHAR)
            {
                Log(ERROR, "[4XX]: cannot parse host from request");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }

            char *second_carriage_return_ptr = strchr(colon_ptr, CARRIAGE_CHAR);
            if (*second_carriage_return_ptr != CARRIAGE_CHAR)
            {
                Log(ERROR, "[4XX]: couldn't find 2nd carriage return");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
            size_t expected_port_length = second_carriage_return_ptr - (colon_ptr + 1);
            expected_port_length++; // NULL_CHAR
            request->port = ParsePort(colon_ptr + 1, expected_port_length);
            if (request->port == ERROR_PORT)
            {
                Log(ERROR, "[4XX]: couldn't parse port");
                request->bail_resp_code = HttpBadRequest;
                return request;
            }
        }
        // Headers and Body
        else if (line_count >= 3 && current_line_in_http_request != NULL)
        {
            // printf("[%s]\n", current_line_in_http_request);
            if (!headers_map_created)
            {
                Log(TRACE, "creating headers hashmap!");
                request->headers = DefaultHashMapInit();
                if (request->headers == NULL)
                {
                    Log(ERROR, "[5XX]: not enough memory to create hashmap for request headers");
                    request->bail_resp_code = HttpBadGateway;
                    return request;
                }
                headers_map_created = true;
            }

            if (*current_line_in_http_request == CARRIAGE_CHAR && *(current_line_in_http_request + 1) == NULL_CHAR)
            {
                Log(TRACE, "found last line of http request! possible body after");
                found_last_line = true;
            }
            else
            {
                if (!found_last_line && headers_map_created)
                {
                    Log(TRACE, "headers found!");
                    JSONValue *single_header_obj = ParseHeader(current_line_in_http_request);
                    if (single_header_obj == NULL)
                    {
                        // how to know if invalid char in header?
                        // or memory error?
                        Log(ERROR, "[5XX]: not enough memory to parse request headers");
                        request->bail_resp_code = HttpBadGateway;
                        return request;
                    }
                    HashMapInsert(request->headers, single_header_obj);
                }
                else if (found_last_line && parse_body)
                {
                    char *content_length_str = HashMapGetValueDirect(request->headers, "content-length");
                    if (content_length_str == NULL)
                    {
                        Log(ERROR, "[4XX]: couldn't find Content-Length header");
                        request->bail_resp_code = HttpBadRequest;
                        return request;
                    }
                    u_int64_t content_length = strtoll(content_length_str, NULL, 10);
                    if (content_length == 0)
                    {
                        Log(WARN, "content_length is zero");
                    }
                    request->body = ParseBody(HashMapGetValueDirect(request->headers, "content-type"), current_line_in_http_request, content_length);
                    break;
                }
            }
        }

        if (next_line_in_http_request != NULL)
        {
            *next_line_in_http_request = NEWLINE_CHAR; // then restore newline-char
            line_count++;
            current_line_in_http_request = next_line_in_http_request + 1;
        }
        else
        {
            current_line_in_http_request = NULL;
        }
    }

    // PrintHttpRequest(request);
    // FreeHttpRequest(request);
    // exit(0);

    request->path_params = NULL; // can't parse this yet

    return request;
}

void FreeHttpRequest(HttpRequest *request)
{
    if (request != NULL)
    {
        if (request->path != NULL)
        {
            free(request->path);
        }
        if (request->query_params != NULL)
        {
            FreeHashMap(request->query_params);
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
            FreeHashMap(request->headers);
        }
        if (request->path_params != NULL)
        {
            FreeHashMap(request->path_params);
        }
        if (request->body != NULL)
        {
            // FIXME: check body type
            // free(request->body);
            FreeJSON(request->body);
        }
        free(request);
    }
}

// FIXME user logger here
void PrintHttpRequest(HttpRequest *request)
{
    if (request->method != HttpFAKE)
    {
        printf("[DEBUG][METHOD]: %s\n", HttpMethodToStr(request->method));
    }
    if (request->host != NULL)
    {
        printf("[DEBUG][HOST]: %s\n", request->host);
    }
    if (request->port != 0)
    {
        printf("[DEBUG][PORT]: %d\n", request->port);
    }
    if (request->path != NULL)
    {
        printf("[DEBUG][PATH]: %s\n", request->path);
    }
    if (request->query_params != NULL)
    {
        printf("[DEBUG][QUERY_PARAMS]: ");
        PrintHashMap(request->query_params);
        printf("\n");
    }
    if (request->headers != NULL)
    {
        printf("[DEBUG][HEADERS]: ");
        PrintHashMap(request->headers);
        printf("\n");
    }
    if (request->body != NULL)
    {
        printf("[DEBUG][BODY]: ");
        PrintJSON(request->body);
        printf("\n");
    }
}
