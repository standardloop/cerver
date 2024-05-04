#ifndef ROUTER_H
#define ROUTER_H
#include "./request/request.h"
#include "./response/response.h"

#define ROUTER_ERROR -1

typedef void(RouteHandler)(HttpRequest *, HttpResponse *);

// enum PathParamType
// {
//     PARAM_BOOL,
//     PARAM_INT,
//     PARAM_FLOAT,
//     PARAM_STRING
// };

// typedef struct
// {
//     enum PathParamType param_type;
//     int path_index;

// } RouteParam;

typedef struct route
{
    struct route *next;
    char *path;
    // RouteParam *route_params;
    char *route_regex;
    RouteHandler *handler;
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
    RouteTable *head;
    RouteTable *get;
    RouteTable *options;
    RouteTable *post;
    RouteTable *put;
    RouteTable *delete;
    RouteTable *connect;
    RouteTable *patch;
    RouteTable *trace;
} Router;

Router *InitRouter();

RouteTable *InitRouteTable(enum HttpMethod, int);
void PrintRouteTable(RouteTable *tabe);
int AddRouteToTable(RouteTable *, char *, RouteHandler *);

Route *GetRouteFromTable(RouteTable *, char *);

#endif
