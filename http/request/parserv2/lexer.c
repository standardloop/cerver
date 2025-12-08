#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./parser.h"
#include <standardloop/util.h>
#include <standardloop/logger.h>

// general
static void advanceChar(HTTPLexer *);
static void backtrackChar(HTTPLexer *);
static void copyString(char *, char *, size_t, size_t);
static char peakCharHTTPLexer(HTTPLexer *);
static bool isSpecialCharacter(char);

// method
static bool isMethodValid(char *, size_t);
// static bool isFirstCharOfMethod(char);
// static char *makeMethodLiteral(HTTPLexer *);

// uri
static char *makeURILiteral(HTTPLexer *);

// version
static char *makeVersionLiteral(HTTPLexer *);

// used for header name and value
static char *makeStringLiteral(HTTPLexer *);

// body
static char *makeBodyLiteral(HTTPLexer *);

// debug
// static void printLexer(HTTPLexer *);

static void copyString(char *src, char *des, size_t len, size_t src_offset)
{
    if (src == NULL || des == NULL || len <= 0)
    {
        return;
    }

    char *src_it = src + src_offset;
    size_t size = 0;
    while (size < len)
    {
        *des = *src_it;
        des++;
        src_it++;
        size++;
    }
}

extern HTTPLexer *HTTPLexerInit(char *input)
{
    HTTPLexer *lexer = malloc(sizeof(HTTPLexer));
    if (lexer == NULL)
    {
        return NULL;
    }
    lexer->input = input;
    lexer->input_len = strlen(input);
    lexer->current_char = NULL_CHAR;
    lexer->position = -2;
    lexer->read_position = -1;
    lexer->line = 1;
    lexer->last_token = HTTPTokenIllegal;
    lexer->double_crlf = false;
    lexer->error.is_client = false;
    lexer->error.is_server = false;
    lexer->error.message = NULL;

    advanceChar(lexer);

    return lexer;
}

static void advanceChar(HTTPLexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
    {
        lexer->current_char = NULL_CHAR;
    }
    else
    {
        lexer->current_char = lexer->input[lexer->read_position];
    }
    lexer->position = lexer->read_position;
    lexer->read_position++;
}

static void backtrackChar(HTTPLexer *lexer)
{
    lexer->position -= 2;
    lexer->read_position--;
    lexer->current_char = lexer->input[lexer->read_position];
}

extern HTTPToken *NewHTTPToken(enum HTTPTokenType type, u_int32_t start, u_int32_t end, u_int32_t line_num, char *literal)
{
    HTTPToken *token = malloc(sizeof(HTTPToken));
    if (token == NULL)
    {
        return NULL;
    }
    token->type = type;
    token->start = start;
    token->end = end;
    token->literal = literal;
    token->line = line_num;
    return token;
}

// static bool isFirstCharOfMethod(char c)
// {
//     // add to header?
//     if (c == 'G' || c == 'H' || c == 'P' || c == 'D' || c == 'C' || c == 'O' || c == 'T')
//     {
//         return true;
//     }
//     return false;
// }

static bool isMethodLengthValid(size_t len)
{
    if (len == HTTP_GET_LENGTH || len == HTTP_PUT_LENGTH || len == HTTP_POST_LENGTH || len == HTTP_HEAD_LENGTH || len == HTTP_PATCH_LENGTH || len == HTTP_TRACE_LENGTH || len == HTTP_DELETE_LENGTH || len == HTTP_OPTIONS_LENGTH || len == HTTP_CONNECT_LENGTH)
    {
        return true;
    }
    return false;
}

static bool isMethodValid(char *suspected_method, size_t suspected_method_len)
{
    if (suspected_method == NULL || !isMethodLengthValid(suspected_method_len))
    {
        return false;
    }
    // GET or PUT
    if (suspected_method_len == 3)
    {
        return strncmp(suspected_method, HTTP_GET_STRING, suspected_method_len) == 0 || strncmp(suspected_method, HTTP_PUT_STRING, suspected_method_len) == 0;
    }
    // POST or HEAD
    else if (suspected_method_len == 4)
    {
        return strncmp(suspected_method, HTTP_POST_STRING, suspected_method_len) == 0 || strncmp(suspected_method, HTTP_HEAD_STRING, suspected_method_len) == 0;
    }
    // PATCH or TRACE
    else if (suspected_method_len == 5)
    {
        return strncmp(suspected_method, HTTP_PATCH_STRING, suspected_method_len) == 0 || strncmp(suspected_method, HTTP_TRACE_STRING, suspected_method_len) == 0;
    }
    // DELETE
    else if (suspected_method_len == 6)
    {
        return strncmp(suspected_method, HTTP_DELETE_STRING, suspected_method_len) == 0;
    }
    // OPTIONS or CONNECT
    else if (suspected_method_len == 7)
    {
        return strncmp(suspected_method, HTTP_OPTIONS_STRING, suspected_method_len) == 0 || strncmp(suspected_method, HTTP_CONNECT_STRING, suspected_method_len) == 0;
    }
    return false;
}

// static char *makeMethodLiteral(HTTPLexer *lexer)
// {
//     if (lexer == NULL)
//     {
//         return NULL;
//     }
//     u_int32_t start_position = lexer->position;
//     // bool is_error = false;
//     u_int8_t max_method_length = 7;
//     u_int8_t start_till_max = 0;
//     while (ALWAYS)
//     {
//         // printf("curr char: %d\n", lexer->current_char);
//         advanceChar(lexer);
//         start_till_max++;
//         // if (lexer->current_char == SPACE_CHAR || lexer->current_char == NULL_CHAR || start_till_max == max_method_length)
//         if (isSpecialCharacter(lexer->current_char) || start_till_max == max_method_length)
//         {
//             break;
//         }
//     }

//     u_int32_t method_literal_size = (lexer->position - start_position) + 1;
//     char *method_literal = malloc(sizeof(char) * method_literal_size);
//     if (method_literal == NULL)
//     {
//         // ERRNO
//         Log(ERROR, "couldn't allocated memory for method_literal");
//         lexer->error.is_server = true;
//         lexer->error.message = "couldn't allocated memory for method_literal";
//         return NULL;
//     }
//     copyString(lexer->input, method_literal, method_literal_size, start_position);
//     method_literal[method_literal_size - 1] = NULL_CHAR;
//     if (!isMethodValid(method_literal, strlen(method_literal)))
//     {
//         // ERRNO
//         Log(WARN, "lexer says method is invalid");
//         lexer->error.is_client = true;
//         lexer->error.message = "lexer says method is invalid";
//         return NULL;
//     }
//     // Log(TRACE, "method %s is valid for lexer", method_literal);
//     if (lexer->current_char == SPACE_CHAR)
//     {
//         backtrackChar(lexer);
//     }
//     return method_literal;
// }

static char *makeURILiteral(HTTPLexer *lexer)
{
    if (lexer == NULL)
    {
        return NULL;
    }
    u_int32_t start_position = lexer->position;
    u_int16_t max_uri_length = MAX_URI_LENGTH;
    u_int16_t start_till_max = 0;
    while (ALWAYS)
    {
        // printf("curr char: %c\n", lexer->current_char);
        advanceChar(lexer);
        start_till_max++;
        // if (lexer->current_char == SPACE_CHAR || lexer->current_char == NULL_CHAR || start_till_max == max_uri_length)
        if (isSpecialCharacter(lexer->current_char) || start_till_max == max_uri_length)
        {
            break;
        }
    }
    u_int32_t uri_literal_size = (lexer->position - start_position) + 1;
    char *uri_literal = malloc(sizeof(char) * uri_literal_size);
    if (uri_literal == NULL)
    {
        // ERRNO
        // Log(ERROR, "couldn't allocated memory for uri_literal");
        return NULL;
    }
    copyString(lexer->input, uri_literal, uri_literal_size, start_position);
    uri_literal[uri_literal_size - 1] = NULL_CHAR;
    // Log(TRACE, "uri %s is valid for lexer", uri_literal);

    if (lexer->current_char == SPACE_CHAR)
    {
        backtrackChar(lexer);
    }

    return uri_literal;
}

static char *makeVersionLiteral(HTTPLexer *lexer)
{
    if (lexer == NULL)
    {
        return NULL;
    }
    u_int32_t start_position = lexer->position;
    u_int16_t max_version_length = MAX_VERSION_LENGTH;
    u_int16_t start_till_max = 0;

    bool found_slash = false;
    while (ALWAYS)
    {
        advanceChar(lexer);
        start_till_max++;
        if (lexer->current_char == FORWARDLASH_CHAR)
        {
            found_slash = true;
        }
        // if (lexer->current_char == NULL_CHAR || lexer->current_char == CARRIAGE_CHAR || start_till_max == max_version_length)
        if (isSpecialCharacter(lexer->current_char) || start_till_max == max_version_length)
        {
            break;
        }
    }
    if (!found_slash)
    {
        Log(ERROR, "Lexer didn't find / in http version");
        return NULL;
    }
    u_int32_t version_literal_size = (lexer->position - start_position) + 1;
    // HTTP/2\0
    if (version_literal_size < 7)
    {
        Log(ERROR, "Lexer http version_literal_size is too small!");
        return NULL;
    }
    char *version_literal = malloc(sizeof(char) * version_literal_size);
    if (version_literal == NULL)
    {
        // ERRNO
        // Log(ERROR, "couldn't allocated memory for version_literal");
        return NULL;
    }

    copyString(lexer->input, version_literal, version_literal_size, start_position);
    version_literal[version_literal_size - 1] = NULL_CHAR;

    if (strncmp(version_literal, "HTTP/", 5))
    {
        Log(ERROR, "Lexer first 5 characters of version_literal were not HTTP/");
        free(version_literal);
        return NULL;
    }
    // Log(TRACE, "version %s is valid for lexer", version_literal);

    // if (isSpecialCharacter(lexer->current_char))
    if (lexer->current_char == CARRIAGE_CHAR || lexer->current_char == SPACE_CHAR)
    {
        backtrackChar(lexer);
    }

    return version_literal;
}

static char *makeStringLiteral(HTTPLexer *lexer)
{
    if (lexer == NULL)
    {
        return NULL;
    }
    u_int32_t start_position = lexer->position;
    u_int16_t max_header_length = MAX_HEADER_LENGTH;
    u_int16_t start_till_max = 0;
    while (ALWAYS)
    {
        // printf("curr char: %c\n", lexer->current_char);
        advanceChar(lexer);
        start_till_max++;
        // if (lexer->current_char == CARRIAGE_CHAR || lexer->current_char == COLON_CHAR || lexer->current_char == SPACE_CHAR || lexer->current_char == NULL_CHAR || start_till_max == max_uri_length)
        // && peakCharHTTPLexer(lexer) == SPACE_CHAR
        if (start_till_max == max_header_length)
        {
            break;
        }
        if (isSpecialCharacter(lexer->current_char))
        {
            // try to handle port
            if (lexer->current_char == COLON_CHAR && peakCharHTTPLexer(lexer) != SPACE_CHAR)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }

    u_int32_t string_literal_size = (lexer->position - start_position) + 1;
    char *string_literal = malloc(sizeof(char) * string_literal_size);
    if (string_literal == NULL)
    {
        // ERRNO
        // Log(ERROR, "couldn't allocated memory for string_header_literal");
        return NULL;
    }
    copyString(lexer->input, string_literal, string_literal_size, start_position);
    string_literal[string_literal_size - 1] = NULL_CHAR;
    // Log(TRACE, "string %s is valid for lexer", string_literal);

    if (lexer->current_char == CARRIAGE_CHAR || lexer->current_char == COLON_CHAR || lexer->current_char == SPACE_CHAR)
    {
        backtrackChar(lexer);
    }
    return string_literal;
}

static bool isSpecialCharacter(char c)
{
    return c == NEWLINE_CHAR || c == COLON_CHAR || c == NULL_CHAR || c == CARRIAGE_CHAR || c == SPACE_CHAR;
}

// static void printLexer(HTTPLexer *lexer)
// {
//     printf("input_len: %d\n", lexer->input_len);
//     printf("current_char: %c\n", lexer->current_char);
//     printf("position: %d\n", lexer->position);
//     printf("read_position: %d\n", lexer->read_position);
//     printf("line: %d\n", lexer->line);
// }

static char peakCharHTTPLexer(HTTPLexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
    {
        return NULL_CHAR;
    }
    return lexer->input[lexer->read_position];
}

static char *makeBodyLiteral(HTTPLexer *lexer)
{
    if (lexer == NULL)
    {
        return NULL;
    }
    u_int32_t start_position = lexer->position;
    u_int64_t max_body_length = MAX_BODY_LENGTH;
    u_int64_t start_till_max = 0;
    while (ALWAYS)
    {
        // printf("curr char: %c\n", lexer->current_char);
        advanceChar(lexer);
        start_till_max++;
        if (lexer->current_char == NULL_CHAR || start_till_max == max_body_length)
        {
            break;
        }
    }

    u_int32_t body_literal_size = (lexer->position - start_position) + 1;
    char *body_literal = malloc(sizeof(char) * body_literal_size);
    if (body_literal == NULL)
    {
        // ERRNO
        // Log(ERROR, "couldn't allocated memory for string_header_literal");
        return NULL;
    }
    copyString(lexer->input, body_literal, body_literal_size, start_position);
    body_literal[body_literal_size - 1] = NULL_CHAR;
    // Log(TRACE, "string %s is valid for lexer", body_literal);

    if (lexer->current_char == NULL_CHAR)
    {
        backtrackChar(lexer);
    }
    return body_literal;
}

extern HTTPToken *HTTPLex(HTTPLexer *lexer)
{
    HTTPToken *token = NULL;

    advanceChar(lexer);

    u_int32_t curr_pos = lexer->position;

    if (lexer->current_char == SPACE_CHAR)
    {
        token = NewHTTPToken(HTTPTokenSpace, curr_pos, lexer->position + 1, lexer->line, NULL);
        lexer->last_token = HTTPTokenSpace;
    }
    else if (lexer->current_char == FORWARDLASH_CHAR && lexer->line == 1 && lexer->last_token == HTTPTokenSpace)
    {
        char *uri_literal = makeURILiteral(lexer);
        if (uri_literal == NULL)
        {
            token = NewHTTPToken(HTTPTokenIllegal, curr_pos, lexer->position + 1, lexer->line, NULL);
            lexer->last_token = HTTPTokenIllegal;
        }
        else
        {
            token = NewHTTPToken(HTTPTokenURI, curr_pos, lexer->position + 1, lexer->line, uri_literal);
            lexer->last_token = HTTPTokenURI;
        }
    }
    else if (lexer->current_char == 'H' && lexer->line == 1 && lexer->last_token == HTTPTokenSpace)
    {
        char *version_literal = makeVersionLiteral(lexer);
        if (version_literal == NULL)
        {
            token = NewHTTPToken(HTTPTokenIllegal, curr_pos, lexer->position + 1, lexer->line, NULL);
            lexer->last_token = HTTPTokenIllegal;
        }
        else
        {
            token = NewHTTPToken(HTTPTokenVersion, curr_pos, lexer->position + 1, lexer->line, version_literal);
            lexer->last_token = HTTPTokenVersion;
        }
    }
    else if (lexer->current_char == CARRIAGE_CHAR)
    {
        if (peakCharHTTPLexer(lexer) == NEWLINE_CHAR)
        {
            lexer->line++;
            advanceChar(lexer);
            token = NewHTTPToken(HTTPTokenCRLF, curr_pos, lexer->position + 1, lexer->line, NULL);
            if (lexer->last_token == HTTPTokenCRLF)
            {
                lexer->double_crlf = true;
            }
            lexer->last_token = HTTPTokenCRLF;
        }
        else
        {
            token = NewHTTPToken(HTTPTokenCarriage, curr_pos, lexer->position + 1, lexer->line, NULL);
            lexer->last_token = HTTPTokenCarriage;
        }
        // else
        // {
        //     token = NewHTTPToken(HTTPTokenIllegal, curr_pos, lexer->position + 1, lexer->line, NULL);
        // }
    }
    else if (lexer->current_char == NEWLINE_CHAR)
    {
        lexer->line++;
        token = NewHTTPToken(HTTPTokenNewline, curr_pos, lexer->position + 1, lexer->line, NULL);
        lexer->last_token = HTTPTokenNewline;
    }
    else if (lexer->current_char == NULL_CHAR)
    {
        token = NewHTTPToken(HTTPTokenEOF, curr_pos, lexer->position + 1, lexer->line, NULL);
        lexer->last_token = HTTPTokenEOF;
    }
    else if (lexer->current_char == COLON_CHAR)
    {
        token = NewHTTPToken(HTTPTokenColon, curr_pos, lexer->position + 1, lexer->line, NULL);
        lexer->last_token = HTTPTokenColon;
    }
    else if (lexer->double_crlf)
    {
        char *body_literal = makeBodyLiteral(lexer);
        if (body_literal == NULL)
        {
            token = NewHTTPToken(HTTPTokenIllegal, curr_pos, lexer->position + 1, lexer->line, NULL);
            lexer->last_token = HTTPTokenIllegal;
        }
        else
        {
            token = NewHTTPToken(HTTPTokenBody, curr_pos, lexer->position + 1, lexer->line, body_literal);
            lexer->last_token = HTTPTokenBody;
        }
    }
    else
    {
        char *string_literal = makeStringLiteral(lexer);
        if (string_literal == NULL)
        {
            token = NewHTTPToken(HTTPTokenIllegal, curr_pos, lexer->position + 1, lexer->line, NULL);
            lexer->last_token = HTTPTokenIllegal;
        }
        else if (lexer->line == 1 && isMethodValid(string_literal, strlen(string_literal)))
        {
            token = NewHTTPToken(HTTPTokenMethod, curr_pos, lexer->position + 1, lexer->line, string_literal);
            lexer->last_token = HTTPTokenMethod;
        }
        else
        {
            token = NewHTTPToken(HTTPTokenString, curr_pos, lexer->position + 1, lexer->line, string_literal);
            lexer->last_token = HTTPTokenString;
        }
    }
    return token;
}

extern void FreeHTTPToken(HTTPToken *token)
{
    // MEMORY: NOTE, doesn't free the literl
    if (token != NULL)
    {
        free(token);
    }
}

extern void FreeHTTPLexer(HTTPLexer *lexer)
{
    if (lexer != NULL)
    {
        free(lexer);
    }
}

extern void HTTPLexerDebugTest(char *input_str, bool exit_after)
{
    HTTPLexer *lexer = HTTPLexerInit(input_str);

    while (ALWAYS)
    {
        HTTPToken *token = HTTPLex(lexer);
        if (token != NULL)
        {
            PrintHTTPToken(token, true);
            if (token->type == HTTPTokenEOF)
            {
                FreeHTTPToken(token);
                break;
            }
        }
        FreeHTTPToken(token);
    }
    FreeHTTPLexer(lexer);
    if (exit_after)
    {
        exit(0);
    }
}

extern void PrintHTTPToken(HTTPToken *token, bool print_literal)
{
    if (token == NULL)
    {
        return;
    }
    printf("Line: %u Place: %u - %u ", token->line, token->start, token->end);

    switch (token->type)
    {
    case HTTPTokenMethod:
        printf("Kind: HTTPTokenMethod");
        break;
    case HTTPTokenSpace:
        printf("Kind: HTTPTokenSpace");
        break;
    case HTTPTokenURI:
        printf("Kind: HTTPTokenURI");
        break;
    case HTTPTokenVersion:
        printf("Kind: HTTPTokenVersion");
        break;
    case HTTPTokenNewline:
        printf("Kind: HTTPTokenNewline");
        break;
    case HTTPTokenCarriage:
        printf("Kind: HTTPTokenCarriage");
        break;
    case HTTPTokenCRLF:
        printf("Kind: HTTPTokenCRLF");
        break;
    case HTTPTokenColon:
        printf("Kind: HTTPTokenColon");
        break;
    case HTTPTokenString:
        printf("Kind: HTTPTokenString");
        break;
    case HTTPTokenEOF:
        printf("Kind: HTTPTokenEOF");
        break;
    case HTTPTokenBody:
        printf("Kind: HTTPTokenBody");
        break;
    case HTTPTokenIllegal:
    default:
        printf("Kind: HTTPTokenIllegal");
        break;
    }
    if (print_literal && token->literal)
    {
        printf(" Literal: %s\n", token->literal);
    }
    else
    {
        printf("\n");
    }
}
