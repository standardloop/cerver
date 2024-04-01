#include <stdlib.h>
#include <stdbool.h>

#include "./router.h"
#include "./../logger.h"

void freeRoute(Route *);
void freeRouteTable(RouteTable *);
bool isRouteTableEmpty(RouteTable *);

RouteTable *InitRouteTable(int max, enum HttpMethod method)
{
    RouteTable *route_table = malloc(sizeof(RouteTable));
    if (route_table == NULL)
    {
        return NULL;
    }
    route_table->size = 0;
    route_table->max = max;
    route_table->method = method;
    route_table->routes = NULL;

    return route_table;
}

Route *newRoute(char *path, RouterFunction *router_function(HttpRequest *, HttpResponse *))
{
    Route *route = (Route *)malloc(sizeof(Route));
    if (route == NULL)
    {
        return NULL;
    }
    route->next = NULL;
    route->handler = router_function;
    route->path = path;

    return route;
}

bool isRouteTableEmpty(RouteTable *route_table)
{
    return (route_table->routes == NULL || route_table->size == 0);
}

bool isRouteTableFull(RouteTable *route_table)
{
    return (route_table->max == route_table->size);
}

int AddRouteToTable(RouteTable *route_table, enum HttpMethod method, char *path,
                    RouterFunction *router_function(HttpRequest *, HttpResponse *))
{
    if (route_table == NULL ||
        path == NULL ||
        route_table->method != method ||
        route_table->method == HttpFAKER)
    {
        return -420;
    }

    Route *route = newRoute(path, router_function);
    if (route == NULL)
    {
        return -420;
    }

    Route *head = route_table->routes;
    if (route_table->routes == NULL)
    {
        route_table->routes = head;
        route_table->size = 1;
        return route_table->size;
    }

    Route *last = route_table->routes;

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = route;
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
