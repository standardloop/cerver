#include <stdlib.h>
#include <stdio.h>

#include "./response.h"

void FreeHttpResponse(HttpResponse *response)
{
    free(response);
}
