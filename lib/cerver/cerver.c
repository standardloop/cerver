#include "cerver.h"

#include <stdio.h>      // perror
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()

#include <string.h> // fixme

// #include "../httpcodes/httpcodes.h"

#include "./http/request/request.h"
#include "./util/util.h"

int cerverLoop(int, struct sockaddr_in, int); // FIXME: where to put this declaration?

int Cerver(int port)
{
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // TODO: re-use options for this
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {

        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    return cerverLoop(server_fd, address, addrlen);
}

int cerverLoop(int server_fd, struct sockaddr_in address, int addrlen)
{
    int new_socket;
    ssize_t valread;
    //  Make this dynamic :D
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    // long served_count = 0;
    size_t buffer_size = BUFFER_SIZE;
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("[FATAL]: Couldn't accept connections on socket?");
            exit(EXIT_FAILURE);
        }

        char *buffer = malloc(sizeof(char) * buffer_size); // no malloc here?
        if (buffer == NULL)
        {
            perror("[FATAL]: Couldn't allocate memory to to find ContentLength");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, sizeof(char) * buffer_size);
        if (valread == 0)
        {
            printf("\n[FATAL]: Didn't read more than 0\n");
            free(buffer);
            exit(EXIT_FAILURE);
        }
        // PrintBuffer(buffer);

        HttpRequest *request = CreateHttpRequest(buffer, buffer_size); // pass valread here?

        if (request == NULL)
        {
            // if you cannot parse the request, we need to return a 4XX?
            free(buffer);
            printf("\n[ERROR]: HttpRequest faile to parse\n");
            // exit(EXIT_FAILURE);
        }
        free(buffer);

        // DO SOMETHING WITH THE REQUEST STRUCT

        FreeHttpRequest(request);
        write(new_socket, hello, strlen(hello));
        // served_count++;
        close(new_socket);
    }
    // FIXME
    return EXIT_SUCCESS;
}
