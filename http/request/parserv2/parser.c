#include "./parser.h"
#include "../request.h"

// static void nextHTTPToken(HTTPParser *);

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
    parser->previous_token = NULL;
    parser->current_token = NULL;
    parser->peek_token = NULL;

    NextHTTPToken(parser);

    return parser;
}

extern void NextHTTPToken(HTTPParser *parser)
{
    if (parser == NULL)
    {
        return;
    }
    FreeHTTPToken(parser->previous_token);
    parser->previous_token = parser->current_token;
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
        if (parser->previous_token != NULL)
        {
            FreeHTTPToken(parser->previous_token);
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
