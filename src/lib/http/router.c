#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "./router.h"
#include <standardloop/logger.h>
#include <standardloop/util.h>

#define DIGIT_REGEX_SIZE 12 // [[:digit:]]+
#define ALPHA_REGEX_SIZE 12 // [[:alpha:]]+

static char *DIGIT_REGEX = "[[:digit:]]+";
static char *ALPHA_REGEX = "[[:alpha:]]+";

static char *DIGIT_REGEX_W_SLASH = "[[:digit:]]+/";
static char *ALPHA_REGEX_W_SLASH = "[[:alpha:]]+/";

static void freeRoute(Route *);
static bool isRouteTableEmpty(RouteTable *);

static char *createRouteRegex(char *path);
// static bool isRouteTableFull(RouteTable *);

HashMap *ParsePathParams(RouteParam *params)
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

static char *createRouteRegex(char *path)
{
    StringArr *exploded_path = EveryoneExplodeNow(path, FORWARDLASH_CHAR);
    if (exploded_path == NULL || exploded_path->num_strings == 0 || strlen(exploded_path->strings[0]) != 0 || exploded_path->strings[0][0] != NULL_CHAR)
    {
        Log(FATAL, "invalid inputs to createRouteRegex");
        return NULL;
    }
    size_t regex_route_size = 4; // '^' , '/' , '$', NULL_CHAR
    size_t regex_route_curr_index = regex_route_size - 2;
    char *regex_route = malloc(sizeof(char) * regex_route_size);
    if (regex_route == NULL)
    {
        // log
        Log(FATAL, "cannot allocate memory :(");
        return NULL;
    }
    regex_route[0] = CARROT_CHAR;
    regex_route[1] = FORWARDLASH_CHAR;

    for (int i = 1; i < exploded_path->num_strings; i++)
    {
        size_t curr_string_len = strlen(exploded_path->strings[i]);
        char *curr_string = exploded_path->strings[i];
        char *curr_string_start = exploded_path->strings[i];
        if (curr_string[0] == '{' && curr_string[curr_string_len - 1] == '}')
        {
            char *param_type_start = NULL;
            char *param_type_end = curr_string + curr_string_len - 1;
            char *param_name_start = curr_string + 1;
            char *param_name_end = NULL;
            for (size_t j = 1; j < curr_string_len; j++)
            {
                if (*curr_string == '=')
                {
                    param_name_end = curr_string;
                    param_type_start = curr_string + 1;
                    break;
                }
                curr_string++;
            }

            size_t param_name_size = (param_name_end - param_name_start) + 1;
            size_t param_type_size = (param_type_end - param_type_start) + 1;

            char *param_type = malloc(sizeof(char) * param_type_size);
            if (param_type == NULL)
            {
                // log
            }
            CopyStringServer(curr_string + 1, param_type, param_type_size - 1, 0, false);
            *(param_type + param_type_size) = NULL_CHAR;

            curr_string = curr_string_start;

            char *param_name = malloc(sizeof(char) * param_name_size);
            if (param_name == NULL)
            {
                // log
            }
            CopyStringServer(curr_string + 1, param_name, param_name_size - 1, 0, false);
            *(param_name + param_name_size) = NULL_CHAR;
            // printf("[param_name]: %s %d\n", param_name, (int)param_name[strlen(param_name)]);
            // printf("[param_type]: %s %d\n", param_type, (int)param_type[strlen(param_type)]);
            if (strcmp("string", param_type) == 0)
            {
                regex_route_size += ALPHA_REGEX_SIZE;
                if (i != exploded_path->num_strings - 1)
                {
                    regex_route_size++;
                }
                regex_route = realloc(regex_route, regex_route_size);
                if (i != exploded_path->num_strings - 1)
                {
                    CopyStringServer(ALPHA_REGEX_W_SLASH, regex_route, ALPHA_REGEX_SIZE + 1, regex_route_curr_index, false);
                    regex_route_curr_index += ALPHA_REGEX_SIZE + 1;
                }
                else
                {
                    CopyStringServer(ALPHA_REGEX, regex_route, ALPHA_REGEX_SIZE, regex_route_curr_index, false);
                    regex_route_curr_index += ALPHA_REGEX_SIZE;
                }
            }
            else if (strcmp("int", param_type) == 0)
            {
                regex_route_size += DIGIT_REGEX_SIZE;
                if (i != exploded_path->num_strings - 1)
                {
                    regex_route_size++;
                }
                regex_route = realloc(regex_route, regex_route_size);
                if (i != exploded_path->num_strings - 1)
                {
                    CopyStringServer(DIGIT_REGEX_W_SLASH, regex_route, DIGIT_REGEX_SIZE + 1, regex_route_curr_index, false);
                    regex_route_curr_index += DIGIT_REGEX_SIZE + 1;
                }
                else
                {
                    CopyStringServer(DIGIT_REGEX, regex_route, DIGIT_REGEX_SIZE, regex_route_curr_index, false);
                    regex_route_curr_index += DIGIT_REGEX_SIZE;
                }
            }
            // FIXME, add these to route
            free(param_name);
            free(param_type);
        }
        else
        {
            regex_route_size += curr_string_len;
            if (i != exploded_path->num_strings - 1)
            {
                regex_route_size++;
            }
            regex_route = realloc(regex_route, regex_route_size);
            if (i != exploded_path->num_strings - 1)
            {
                curr_string[curr_string_len] = FORWARDLASH_CHAR;
                CopyStringServer(curr_string, regex_route, curr_string_len + 1, regex_route_curr_index, false);
                regex_route_curr_index += curr_string_len + 1;
            }
            else
            {
                CopyStringServer(curr_string, regex_route, curr_string_len, regex_route_curr_index, false);
                regex_route_curr_index += curr_string_len;
            }
        }
    }

    regex_route[regex_route_size - 2] = DOLLAR_CHAR;
    regex_route[regex_route_size - 1] = NULL_CHAR;

    FreeStringArr(exploded_path);

    return regex_route;
}

static Route *newRoute(char *path, RouteHandler *router_function)
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

static bool isRouteTableEmpty(RouteTable *table)
{
    return (table->routes == NULL || table->size == 0);
}

// static bool isRouteTableFull(RouteTable *table)
// {
//     return (table->max == table->size);
// }

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

static void freeRoute(Route *route)
{
    if (route == NULL)
    {
        return;
    }

    if (route->route_regex != NULL)
    {
        // FIXME
        // free(route->route_regex);
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
