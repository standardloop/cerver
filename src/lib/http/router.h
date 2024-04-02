#ifndef ROUTER_H
#define ROUTER_H
#include "./request/request.h"
#include "./response/response.h"

typedef void(RouterFunction)(HttpRequest *, HttpResponse *);

typedef struct route
{
    struct route *next;
    char *path;
    RouterFunction *handler;
} Route;

typedef struct
{
    Route *routes;
    int size;
    int max;
    enum HttpMethod method;
} RouteTable;

typedef struct
{
    RouteTable *get;
    // RouteTable *post;
    // RouteTable *put;
    // RouteTable *delete;
} RouteTableAll;

RouteTableAll *InitRouteTableAll();

RouteTable *InitRouteTable(enum HttpMethod, int);
void PrintRouteTable(RouteTable *tabe);
int AddRouteToTable(RouteTable *, char *, RouterFunction *);

Route *GetRouteFromTable(RouteTable *, char *);

#endif
