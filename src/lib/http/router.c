#include <stdlib.h>

#include "./router.h"
#include "./../logger.h"

void freeRoute(Route *route);

int AddRoute(RouteTable *route_table, enum HttpMethod method, char *path, void (*RouterFunction)(HttpRequest *, HttpResponse *))
{
    if (route_table == NULL ||
        path == NULL ||
        route_table->method != method ||
        route_table->method == HttpFAKER)
    {
        return 0;
    }
    if (RouterFunction == NULL)
    {
        // static
        route_table->size++;
        return route_table->size;
    }

    route_table->size++;
    return route_table->size;
}

void freeRoute(Route *route)
{
    if (route == NULL)
    {
        return;
    }

    if (route->path != NULL)
    {
        free(route->path);
    }
    if (route->handler != NULL)
    {
        free(route->handler);
    }
}

void FreeRouteTable(RouteTable *table)
{
    if (table->routes != NULL)
    {
        // FIXME loop
        freeRoute(table->routes);
    }
}
