#include "./lib/cerver/cerver.h"

int main(int argc, char const *argv[])
{
    // FIXME: more logically start up (also look into the arg parser for fun)
    int port = 8080;
    return Cerver(port);
}
