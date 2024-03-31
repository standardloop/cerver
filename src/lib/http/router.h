#ifndef ROUTER_H
#define ROUTER_H
#include "./request/request.h"
// #include "./request/parser/method.h"

#include "./response/response.h"

typedef void (*RouterFunction)(HttpRequest *req, HttpResponse *res);

typedef struct
{
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

int AddRoute(RouteTable *, enum HttpMethod, char *, void (*RouterFunction)(HttpRequest *, HttpResponse *));

#endif

/*

Want an ability to associate a function with a route(path + method)

Ability to add a simple route such as GET to /hello
void AddRoute(enum HttpMethod method, char * path,  )
{

}

Ability to add a router (like api router)
void AddRouter()
{

}

void RouterTemplate(HttpRequest *req, HttpResponse *res)
{
    if (req == NULL || res == NULL)
    {
        return;
    }
}


*/