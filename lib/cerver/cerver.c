#include "cerver.h"

#include <stdio.h>      // perror
#include <sys/socket.h> // socket() bind() listen() accept() sockaddr
#include <stdlib.h>     // EXIT_FAILURE
#include <string.h>     // strlen()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // write() close()

// #include "../httpcodes/httpcodes.h"

#include "./spec/http/request/request.h"

int cerverLoop(int server_fd, struct sockaddr_in address, int addrlen); // FIXME: where to put this declaration?

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

    // OS will handle cleaning up the port cleanaup
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
    long valread;
    // Make this dynamic :D
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("[FATAL]: Couldn't accept connections on socket?");
            exit(EXIT_FAILURE);
        }

        // buffer until we can see Content-Length?
        char buffer[1024] = {0}; // FIXME: memory lel
        // char *buffer_canary = malloc(sizeof(char *) * 30000);
        // if (buffer_canary == NULL)
        // {
        //     // return 5XX error
        // }
        // free(buffer_canary);
        valread = read(new_socket, buffer, 1024);

        // printf("[DEBUG]: valread: %ld\n", valread); // TODO: logging
        printf("%s\n", buffer);

        // dnb
        HttpRequest *request = CreateHttpRequest();
        // HttpRequest *request = CreateHttpRequest(buffer, valread);
        if (request == NULL)
        {
            // if you cannot parse the request, we need to return a 4XX?
            // perror("[FATAL]: HttpRequest faile to parse ");
            // exit(EXIT_FAILURE);
        }
        /*
            request = ParseRequest(buffer, valread);
            request->headers
            request->body
        */
        write(new_socket, hello, strlen(hello));
        close(new_socket);
    }
    // FIXME
    return EXIT_SUCCESS;
}
