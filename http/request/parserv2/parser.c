#include "./parser.h"
#include "../request.h"

static void nextHTTPToken(HTTPParser *);

extern HTTPParser *HTTPParserInit(HTTPLexer *lexer)
{
    if (lexer == NULL)
    {
        return NULL;
    }

    HTTPParser *parser = malloc(sizeof(HTTPParser));
    if (parser == NULL)
    {
        FreeHTTPLexer(lexer);
        printf("[ERROR]: couldn't allocate memory for HTTPParser\n");
        return NULL;
    }

    parser->input_error = false;
    parser->memory_error = false;
    parser->error_message = NULL;
    parser->lexer = lexer;
    parser->list_nested = 0;
    parser->obj_nested = 0;
    parser->current_token = NULL;
    parser->peek_token = NULL;

    nextHTTPToken(parser);

    return parser;
}

static void nextHTTPToken(HTTPParser *parser)
{
    if (parser == NULL)
    {
        return;
    }
    FreeHTTPToken(parser->current_token);
    parser->current_token = parser->peek_token;
    if (parser->current_token != NULL)
    {
    }
    parser->peek_token = HTTPLex(parser->lexer);
}

extern void FreeHTTPParser(HTTPParser *parser)
{
    if (parser != NULL)
    {
        if (parser->lexer != NULL)
        {
            FreeHTTPLexer(parser->lexer);
        }
        if (parser->current_token != NULL)
        {
            FreeHTTPToken(parser->current_token);
        }
        if (parser->peek_token != NULL)
        {
            FreeHTTPToken(parser->peek_token);
        }
        free(parser);
    }
}

extern HTTPRequest *ParseHTTP(HTTPParser *parser)
{
    if (parser == NULL)
    {
        return NULL;
    }
    HTTPRequest *http_request = malloc(sizeof(HTTPRequest));
    if (http_request == NULL)
    {
        // printf("[ERROR]: Not enough memory for http_request\n");
        FreeHTTPParser(parser);
        return NULL;
    }
    nextHTTPToken(parser);
    while (ALWAYS)
    {
        PrintHTTPToken(parser->current_token, false);
        if (parser->current_token->type == HTTPTokenIllegal)
        {
            FreeHTTPParser(parser);
            return NULL;
        }
        else if (parser->current_token->type == HTTPTokenEOF)
        {
            break;
        }
        nextHTTPToken(parser);
    }

    FreeHTTPParser(parser);
    return http_request;
}
