#include "./lib/cerver/cerver.h"

int main(int argc, char const *argv[])
{
    int port = 8080;
    int status = Cerver(port);
    return (status);
}
