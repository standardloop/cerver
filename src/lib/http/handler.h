#ifndef HANDLER_H
#define HANDLER_H

#include "./router.h"

void HandleRequest(Router *, int);
void HandleGenericError(int, enum HttpCode);

#endif
