#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "./router.h"
#include "./../logger.h"
#include "./../util/util.h"

void freeRoute(Route *);
void freeRouteTable(RouteTable *);
bool isRouteTableEmpty(RouteTable *);

Map *ParsePathParams(RouteParam *params)
{
    if (params == NULL)
    {
        return NULL;
    }
    return NULL;
}

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

/*
    turn:     /foo/{id=int}/bar/{name=string}
    into:     ^/foo/[[:digit:]]+/bar/[[:alpha:]]+$
    to match: /foo/211111/bar/josh
*/

// EARLY WIP
char *createRouteRegex(char *path)
{
    // const char explode_char[2] = "/";
    // char *token;

    // token = strtok(raw_path, explode_char);
    // while (token != NULL)
    // {
    //     printf(" %s\n", token);
    //     token = strtok(NULL, explode_char);
    // }

    return path;
}

Route *newRoute(char *path, RouteHandler *router_function)
{
    Route *route = (Route *)malloc(sizeof(Route));
    if (route == NULL)
    {
        return NULL;
    }
    route->next = NULL;
    route->handler = router_function;
    route->route_regex = createRouteRegex(path);

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
        printf("\n[PrintRouteTable]: %s\n", iterator->route_regex); // FIXME
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
        // FIXME add logic for /{id=int}
        // if (strcmp(path, iterator->path) == 0)
        // {
        //     return iterator;
        // }
        if (RegexBoolMatch(iterator->route_regex, path))
        {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;
}

Router *InitRouter()
{
    Router *router = (Router *)malloc(sizeof(Router));
    if (router == NULL)
    {
        // FIXME LOG
        return NULL;
    }
    router->head = NULL;
    router->get = InitRouteTable(HttpGET, 10);
    router->options = NULL;
    router->post = NULL;
    router->put = NULL;
    router->delete = NULL;
    router->connect = NULL;
    router->patch = NULL;
    router->trace = NULL;

    return router;
}

int AddRouteToTable(RouteTable *table, char *path, RouteHandler *router_function)
{
    // maybe need to check if path is correct syntax?
    if (table == NULL || path == NULL ||
        table->method == HttpFAKE)
    {
        return ROUTER_ERROR;
    }

    Route *route = newRoute(path, router_function);
    if (route == NULL)
    {
        return ROUTER_ERROR;
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

    if (route->route_regex != NULL)
    {
        // FIXME
        free(route->route_regex);
    }
    // Dont need to free
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
