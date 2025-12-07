#ifndef ROUTER_H
#define ROUTER_H

#include "./request/request.h"
#include "./response/response.h"
#include <standardloop/json.h>

#define ROUTER_ERROR -1

typedef void(RouteHandler)(const HTTPRequest *, HTTPResponse *);

typedef struct
{
    char *param_name;
    int path_index;
} RouteParam;

typedef struct route
{
    struct route *next;
    RouteParam *params;
    char *route_regex;
    RouteHandler *handler;
} Route;

typedef struct
{
    Route *routes;
    int size;
    int max;
    enum HTTPMethod method;
} RouteTable;

typedef struct
{
    RouteTable *head;
    RouteTable *get;
    RouteTable *options;
    RouteTable *post;
    RouteTable *put;
    RouteTable *delete;
    RouteTable *connect;
    RouteTable *patch;
    RouteTable *trace;
    // bool add_cerver_header;
} Router;

HashMap *ParsePathParams(RouteParam *);

Router *InitRouter();
void FreeRouter(Router *);

RouteTable *InitRouteTable(enum HTTPMethod, int);
void PrintRouteTable(RouteTable *tabe);
int AddRouteToTable(RouteTable *, char *, RouteHandler *);

Route *GetRouteFromTable(RouteTable *, char *);

#endif
