#include <stdlib.h>
#include <stdio.h>

#include "./response.h"

void FreeHttpResponse(HttpResponse *response)
{
    if (response != NULL)
    {
        free(response);
    }
}
