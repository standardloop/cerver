#include <stdlib.h>

#include "./router.h"
#include "./request/request.h"
#include "./response/response.h"

void RouterTemplate(HttpRequest *req, HttpResponse *res)
{
    if (req == NULL || res == NULL)
    {
        return;
    }
}
