#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "./router.h"
#include "./../logger.h"

void freeRoute(Route *);
void freeRouteTable(RouteTable *);
bool isRouteTableEmpty(RouteTable *);

RouteTable *InitRouteTable(enum HttpMethod method, int max)
{
    RouteTable *table = (RouteTable *)malloc(sizeof(RouteTable));
    if (table == NULL)
    {
        return NULL;
    }
    table->size = 0;
    table->max = max;
    table->method = method;
    table->routes = NULL;

    return table;
}

Route *newRoute(char *path, RouterFunction *router_function)
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

bool isRouteTableEmpty(RouteTable *table)
{
    return (table->routes == NULL || table->size == 0);
}

bool isRouteTableFull(RouteTable *table)
{
    return (table->max == table->size);
}

void PrintRouteTable(RouteTable *table)
{
    if (isRouteTableEmpty(table))
    {
        return;
    }
    Route *iterator = table->routes;
    while (iterator != NULL)
    {
        printf("\n[PrintRouteTable]: %s\n", iterator->path);
        iterator = iterator->next;
    }
}

Route *GetRouteFromTable(RouteTable *table, char *path)
{
    if (isRouteTableEmpty(table))
    {
        return NULL;
    }
    Route *iterator = table->routes;
    while (iterator != NULL)
    {
        // FIXME add logic for /:id or similiar
        // query rip

        if (strcmp(path, iterator->path) == 0)
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

// (RouteTableAll *)malloc(sizeof(RouteTableAll));

RouteTableAll *InitRouteTableAll()
{
    RouteTableAll *router = (RouteTableAll *)malloc(sizeof(RouteTableAll));
    if (router == NULL)
    {
        // FIXME LOG
        return NULL;
    }
    router->get = InitRouteTable(HttpGET, 10);
    return router;
}

int AddRouteToTable(RouteTable *table, char *path,
                    RouterFunction *router_function)
{
    if (table == NULL || path == NULL ||
        table->method == HttpFAKER)
    {
        return -420;
    }

    Route *route = newRoute(path, router_function);
    if (route == NULL)
    {
        return -420;
    }

    Route *head = table->routes;
    if (head == NULL)
    {
        table->routes = route;
        table->size = 1;
        return table->size;
    }

    Route *last = table->routes;

    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = route;
    table->size++;

    return table->size;
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
    // Dont need to freee
    if (route->handler != NULL)
    {
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
