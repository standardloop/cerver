#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // write() close()

#include "./response.h"

void FreeHttpResponse(HttpResponse *response)
{
    if (response != NULL)
    {
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
    response->headers = NULL;
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

