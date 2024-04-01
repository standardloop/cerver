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

RouteTable *InitRouteTable(enum HttpMethod, int);

RouteTableAll *InitRouteTableAll();

int AddRouteToTable(RouteTable *, enum HttpMethod, char *, RouterFunction *);
Route *GetRouteFromTable(RouteTable *, char *);

#endif

/*

Want an ability to associate a function with a route(path + method)

Ability to add a simple route such as GET to /hello
void AddRouteToTable(enum HttpMethod method, char * path,  )
{

}

Ability to add a router (like api router)
void AddRouteToTabler()
{

}


*/