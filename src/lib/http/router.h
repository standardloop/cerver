#ifndef ROUTER_H
#define ROUTER_H
#include "./request/request.h"
// #include "./request/parser/method.h"

#include "./response/response.h"

typedef void(RouterFunction)(HttpRequest *, HttpResponse *);

// Routes a Compile time

typedef struct route
{
    struct route *next;

    char *path;
    RouterFunction *handler;
} Route;

// Create multiple route tables for each method
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
