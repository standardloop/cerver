#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE atoi()
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()
#include <pthread.h>
#include <stdbool.h>

#include "./request.h"
#include "./../response/codes.h"

#include "./parser/parser.h"
#include "./parserv2/parser.h"

#include <standardloop/util.h>
#include <standardloop/logger.h>
#include <standardloop/json.h>

static char *locateQueryStart(char *, size_t);
static HTTPRequest *createBlankHTTPRequest();

static void configureHTTPBody(HTTPRequest *);
static void loopThroughHTTPTokens(HTTPParser *, HTTPRequest *);
static void separatePortFromHost(HTTPRequest *);

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

static HTTPRequest *createBlankHTTPRequest()
{
    HTTPRequest *request = malloc(sizeof(HTTPRequest));
    if (request == NULL)
    {
        return NULL;
    }
    request->client_socket = -1;
    request->method = HTTPGET;
    request->path = NULL;
    request->uri = NULL;
    request->query_params = NULL;
    request->version = NULL;
    request->host = NULL;
    request->port = 80;
    request->bail_resp_code = HTTPCerverDefault;
    request->headers = DefaultHashMapInit();

    // v2
    request->tls = false; // FIXME
    request->host_v2.hostname = NULL;
    request->host_v2.port_number = 80;
    request->method_v2 = V2HTTPGET;
    request->body_v2.literal = NULL;
    request->body_v2.type = HTTPRequestBodyPlain;
    request->error_v2.is_client = false;
    request->error_v2.is_client = false;
    request->error_v2.message = NULL;
    request->error_v2.bail_resp_code = HTTPCerverDefault;

    if (request->headers == NULL)
    {
        Log(ERROR, "DefaultHashMapInit() returned NULL in createBlankHTTPRequest()");
        FreeHTTPRequest(request);
        return NULL;
    }
    request->body = NULL;
    request->path_params = NULL;

    return request;
}

HTTPRequest *CreateParsedHTTPRequest(char *buffer, size_t buffer_size)
{
    // extern HTTPRequest *ParseHTTPRequest(HTTPParser *parser)
    if (buffer == NULL || buffer_size == 0)
    {
        Log(ERROR, "[4XX]: buffer is NULL for CreateParsedHTTPRequest or buffer_size is 0\n");
        return NULL;
    }
    HTTPRequest *request = createBlankHTTPRequest();
    if (request == NULL)
    {
        Log(ERROR, "[5XX]: no memory for malloc-ing HTTPRequest\n");
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
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            size_t suspected_http_method_length = (first_space_pointer - current_line_in_http_request);
            suspected_http_method_length++; // NULL_CHAR

            request->method = ParseRequestMethod(current_line_in_http_request, suspected_http_method_length);
            if (request->method == HTTPINVALID)
            {
                Log(WARN, "[4XX]: Couldn't parse HTTP Request method (HTTPINVALID)\n");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            if (request->method == HTTPPOST || request->method == HTTPPUT)
            {
                parse_body = true;
            }
            // Path and Query
            char *second_space_pointer = strchr((first_space_pointer + 1), SPACE_CHAR); // +1 because pointer is on space
            if ((*second_space_pointer != SPACE_CHAR) || (*first_space_pointer != SPACE_CHAR))
            {
                Log(WARN, "error finding appropriate amount of spaces for path+query");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            size_t suspected_path_length = second_space_pointer - (first_space_pointer + 1);
            suspected_path_length++; // NULL_CHAR
            request->path = ParseRequestPath(first_space_pointer + 1, suspected_path_length);
            if (request->path == NULL)
            {
                Log(WARN, "[4XX]: Couldn't parse HTTP Request path/query\n");
                request->bail_resp_code = HTTPBadGateway;
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
                        request->bail_resp_code = HTTPBadGateway;
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
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            if (*second_space_pointer != SPACE_CHAR)
            {
                Log(WARN, "[4XX]: found carriage return but second space error");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }

            size_t expected_verion_length = carriage_return_ptr - (second_space_pointer + 1);
            expected_verion_length++; // NULL_CHAR
            request->version = ParseHTTPVersion((second_space_pointer + 1), expected_verion_length);
            if (request->version == NULL)
            {
                Log(WARN, "[4XX]: couldn't parser http version");
                request->bail_resp_code = HTTPBadRequest;
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
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            if (*(current_line_in_http_request + 5) != SPACE_CHAR)
            {
                Log(ERROR, "[4XX]: cannot find space after Host:");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            char *suspected_host_start = (current_line_in_http_request + 6); // skip over "Host: "
            char *colon_ptr = strchr(suspected_host_start, COLON_CHAR);
            if (*colon_ptr != COLON_CHAR)
            {
                Log(WARN, "[4XX]: cannot find colon so port wasn't included?");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            size_t expected_host_length = (colon_ptr - suspected_host_start);
            expected_host_length++; // NULL_CHAR
            request->host = ParseHost(suspected_host_start, expected_host_length);
            if (request->host == NULL)
            {
                Log(ERROR, "[4XX]: cannot parse host from request");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }

            // Port
            if (*colon_ptr != COLON_CHAR)
            {
                Log(ERROR, "[4XX]: cannot parse host from request");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }

            char *second_carriage_return_ptr = strchr(colon_ptr, CARRIAGE_CHAR);
            if (*second_carriage_return_ptr != CARRIAGE_CHAR)
            {
                Log(ERROR, "[4XX]: couldn't find 2nd carriage return");
                request->bail_resp_code = HTTPBadRequest;
                return request;
            }
            size_t expected_port_length = second_carriage_return_ptr - (colon_ptr + 1);
            expected_port_length++; // NULL_CHAR
            request->port = ParsePort(colon_ptr + 1, expected_port_length);
            if (request->port == ERROR_PORT)
            {
                Log(ERROR, "[4XX]: couldn't parse port");
                request->bail_resp_code = HTTPBadRequest;
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
                    request->bail_resp_code = HTTPBadGateway;
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
                        request->bail_resp_code = HTTPBadGateway;
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
                        request->bail_resp_code = HTTPBadRequest;
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

    // PrintHTTPRequest(request);
    // FreeHTTPRequest(request);
    // exit(0);

    request->path_params = NULL; // can't parse this yet

    return request;
}

void FreeHTTPRequest(HTTPRequest *request)
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
        // v2
        if (request->host_v2.hostname != NULL)
        {
            free(request->host_v2.hostname);
        }
        if (request->body_v2.literal != NULL)
        {
            switch (request->body_v2.type)
            {
            case HTTPRequestBodyJSON:
                FreeJSON((JSON *)request->body_v2.literal);
                break;
            case HTTPRequestBodyPlain:
                free((char *)request->body_v2.literal);
                break;
            default:
                break;
            }
        }
        if (request->uri != NULL)
        {
            free(request->uri);
        }
        free(request);
    }
}

// FIXME user logger here
void PrintHTTPRequest(HTTPRequest *request)
{
    // print into a buffer so whole thing can work

    // THIS IS ONLY v2 now
    if (request != NULL)
    {
        if (request->method != HTTPINVALID)
        {
            Log(DEBUG, "[METHOD]: %s", HTTPMethodEnumToString(request->method_v2));
        }
        if (request->host_v2.hostname != NULL)
        {
            // printf("[DEBUG][HOST]: %s\n", request->host);
            Log(DEBUG, "[HOST]: %s", request->host_v2.hostname);
        }
        if (request->host_v2.port_number >= 0)
        {
            Log(DEBUG, "[PORT]: %d", request->host_v2.port_number);
        }
        if (request->path != NULL)
        {
            // printf("[DEBUG][PATH]: %s\n", request->path);
        }
        if (request->query_params != NULL)
        {
            // printf("[DEBUG][QUERY_PARAMS]: ");
            // PrintHashMap(request->query_params);
            // printf("\n");
        }
        if (request->headers != NULL)
        {
            char *headers_as_string = ObjToString(request->headers);
            if (headers_as_string != NULL)
            {
                Log(DEBUG, "[HEADERS]: %s", headers_as_string);
                free(headers_as_string);
            }
        }
        if (request->body_v2.literal != NULL)
        {
            if (request->body_v2.type == HTTPRequestBodyJSON)
            {
                char *request_body_json_as_string = JSONToString(request->body_v2.literal, false);
                if (request_body_json_as_string != NULL)
                {
                    Log(DEBUG, "[BODY]: %s", request_body_json_as_string);
                    free(request_body_json_as_string);
                }
            }
            else if (request->body_v2.type == HTTPRequestBodyPlain)
            {
                Log(DEBUG, "[BODY]: %s", (char *)request->body_v2.literal);
            }
        }
    }
}

static void separatePortFromHost(HTTPRequest *http_request)
{
    char *full_host = HashMapGetValueDirect(http_request->headers, "Host");
    if (!IsCharInString(full_host, COLON_CHAR))
    {
        // this should always be true
        // FIXME
        http_request->host_v2.port_number = DEFAULT_HTTP_PORT;
        // if (http_request->tls)
        // {
        //     http_request->host_v2.port_number = DEFAULT_HTTPS_PORT;
        // }
        // else
        // {
        //     http_request->host_v2.port_number = DEFAULT_HTTP_PORT;
        // }
        // return;
    }
    else
    {
        StringArr *host_breakdown = EveryoneExplodeNow(full_host, COLON_CHAR);
        if (host_breakdown == NULL)
        {
            Log(ERROR, "StringArr *host_breakdown = EveryoneExplodeNow(full_host, COLON_CHAR) is NULL!");
        }
        else
        {
            if (host_breakdown->num_strings != 2)
            {
                Log(ERROR, "StringArr *host_breakdown = EveryoneExplodeNow(full_host, COLON_CHAR) is NULL!");
            }
            else
            {
                http_request->host_v2.hostname = QuickAllocatedString(host_breakdown->strings[0]); // copy the string
                http_request->host_v2.port_number = (u_int16_t)atoi(host_breakdown->strings[1]);
            }
        }
        FreeStringArr(host_breakdown);
    }
}

static void loopThroughHTTPTokens(HTTPParser *parser, HTTPRequest *http_request)
{
    // struct to track unique parts of the request
    struct
    {
        bool found_method;
        bool found_uri;
        bool found_version;
        char *header_key;
        bool requires_body;
        bool crlf_found;
        bool double_crlf_found;
    } parser_tracker;

    parser_tracker.found_method = false;
    parser_tracker.found_uri = false;
    parser_tracker.found_version = false;
    parser_tracker.header_key = NULL;
    parser_tracker.requires_body = false;
    parser_tracker.crlf_found = false;
    parser_tracker.double_crlf_found = false;

    NextHTTPToken(parser);

    while (ALWAYS)
    {
        if (!parser_tracker.double_crlf_found)
        {
            parser_tracker.double_crlf_found = parser_tracker.crlf_found && parser->current_token->type == HTTPTokenCRLF;
        }
        parser_tracker.crlf_found = parser->current_token->type == HTTPTokenCRLF;

        // PrintHTTPToken(parser->current_token, true);
        if (parser->current_token->type == HTTPTokenIllegal)
        {
            // FreeHTTPParser(parser);
            // FreeHTTPRequest(http_request);
            Log(ERROR, "HTTPTokenIllegal deteced");
            break;
            // return NULL;
        }
        else if (parser->current_token->type == HTTPTokenCRLF)
        {
        }
        else if (parser->current_token->type == HTTPTokenEOF)
        {
            Log(TRACE, "HTTPTokenEOF deteced");
            break;
        }
        else if (parser->current_token->type == HTTPTokenMethod)
        {
            if (parser_tracker.found_method)
            {
                Log(ERROR, "more than one method found....");
                break;
            }
            else if (parser->peek_token->type != HTTPTokenSpace)
            {
                Log(ERROR, "space not found after method....");
                break;
            }
            http_request->method_v2 = HTTPStringToMethodEnum(parser->current_token->literal);
            free(parser->current_token->literal);
            parser_tracker.requires_body = DoesMethodRequireBody(http_request->method_v2);
            parser_tracker.found_method = true;
        }
        else if (parser->current_token->type == HTTPTokenURI)
        {
            if (parser_tracker.found_uri)
            {
                Log(ERROR, "more than one uri found....");
                break;
            }
            else if (parser->peek_token->type != HTTPTokenSpace)
            {
                Log(ERROR, "space not found after uri....");
                break;
            }
            else
            {
                http_request->uri = parser->current_token->literal;
                parser_tracker.found_uri = true;
            }
        }
        else if (parser->current_token->type == HTTPTokenVersion)
        {
            if (parser_tracker.found_version)
            {
                Log(ERROR, "more than one version found....");
                break;
            }
            else if (parser->peek_token->type != HTTPTokenCRLF)
            {
                Log(ERROR, "no CRLF after version");
                break;
            }
            else
            {
                http_request->version = parser->current_token->literal; // ParseHTTPVersion(parser->current_token->literal, strlen(parser->current_token->literal));
                parser_tracker.found_version = true;
            }
        }
        else if (parser->current_token->type == HTTPTokenBody)
        {
            if (parser_tracker.double_crlf_found && parser_tracker.requires_body && parser->peek_token->type == HTTPTokenEOF)
            {
                http_request->body_v2.literal = parser->current_token->literal;
            }
            else
            {
                if (!parser_tracker.double_crlf_found)
                {
                    Log(ERROR, "double CRLF not found before body");
                }
                if (!parser_tracker.requires_body)
                {
                    Log(ERROR, "double CRLF not found before body");
                }
                if (parser->peek_token->type != HTTPTokenEOF)
                {
                    Log(ERROR, "EOF not found after body");
                }
                break;
            }
        }
        else if (parser->current_token->type == HTTPTokenString)
        {

            if (parser->peek_token->type == HTTPTokenColon)
            {
                if (parser->previous_token->type != HTTPTokenCRLF)
                {
                    // Log(ERROR, "Header key was found, but previous token is not CRLF %d", parser->previous_token->type);
                    // break;
                }
                else
                {
                    parser_tracker.header_key = parser->current_token->literal;
                }
            }
            else if (parser->peek_token->type == HTTPTokenCRLF)
            {
                if (parser->previous_token->type != HTTPTokenSpace)
                {
                    Log(ERROR, "Header value was found, but previous token is not SPACE Char");
                    break;
                }
                else
                {
                    JSONValue *header_entry = JSONValueInit(JSONSTRING_t, parser->current_token->literal, parser_tracker.header_key);
                    if (header_entry == NULL)
                    {
                        Log(ERROR, "FIXME");
                        // free stuff here
                        break;
                    }
                    else
                    {
                        HashMapInsert(http_request->headers, header_entry);
                    }
                }
            }
        }
        else if (parser->current_token->type == HTTPTokenColon)
        {
            if (parser->peek_token->type != HTTPTokenSpace)
            {
                // Log(ERROR, "No space after colon");
                // break;
            }
            else if (parser->previous_token->type != HTTPTokenString)
            {
                Log(ERROR, "No header key string before colon");
                break;
            }
        }
        NextHTTPToken(parser);
    }

    if (!parser_tracker.double_crlf_found)
    {
        http_request->bail_resp_code = HTTPBadRequest;
    }
    else if (parser_tracker.requires_body && http_request->body == NULL)
    {
        // FreeHTTPRequest(http_request);
        //  bail response
        http_request->bail_resp_code = HTTPMethodNotAllowed;
    }
}

// V2
extern HTTPRequest *ParseHTTPRequest(HTTPParser *parser)
{
    if (parser == NULL)
    {
        Log(ERROR, "HTTPParser *parser is NULL!");
        return NULL;
    }
    // if (!parser->lexer->double_crlf)
    // {
    //     Log(ERROR, "Lexer did not found double_crlf");
    //     return NULL;
    // }
    HTTPRequest *http_request = createBlankHTTPRequest();
    if (http_request == NULL)
    {
        Log(ERROR, "HTTPRequest *http_request = createBlankHTTPRequest() is NULL!");
        FreeHTTPParser(parser);
        return NULL;
    }

    // have this return an error maybe?
    loopThroughHTTPTokens(parser, http_request);
    separatePortFromHost(http_request);
    configureHTTPBody(http_request);
    FreeHTTPParser(parser);
    return http_request;
}

static void configureHTTPBody(HTTPRequest *http_request)
{
    if (http_request == NULL)
    {
        return;
    }
    char *http_request_content_type = (char *)HashMapGetValueDirect(http_request->headers, "Content-Type");
    if (http_request_content_type == NULL)
    {
        Log(ERROR, "request requires body but, Content-Type header not found");
    }
    if (strcmp(http_request_content_type, "text/plain") == 0)
    {
        http_request->body_v2.type = HTTPRequestBodyPlain;
    }
    else if (strcmp(http_request_content_type, "application/json"))
    {
        http_request->body_v2.type = HTTPRequestBodyJSON;
        char *current_str_body = http_request->body_v2.literal;
        JSON *body_as_json = StringToJSON(current_str_body);
        if (body_as_json == NULL)
        {
            Log(ERROR, "request requires body but, Content-Type header not found");
        }
        else
        {
        }
    }
}
