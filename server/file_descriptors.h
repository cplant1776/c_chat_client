
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <my_global.h>
#include <mysql.h>

#include "../functions.h"
#include "globals.h"
#include "config.h"

void add_listener_set(int socket_fd, fd_set *read_fd)
{
    //clear the socket set
    FD_ZERO(read_fd);

    //add master socket to set
    FD_SET(socket_fd, read_fd);
}

void add_connection_sets(struct client *client_socket, fd_set *read_fd, int *max_sd)
{
    // loop through all possible connections and find valid ones
    int i, sock_desc;
    for (i=0 ; i < MAXCONNECTIONS ; i++)
    {
        // current socket descriptor
        sock_desc = client_socket[i].socket;

        // add to read fd list if valid
        if(sock_desc > 0)
        {
            FD_SET(sock_desc, read_fd);
        }

        // keep track of highest fd
        if(sock_desc > *max_sd)
            *max_sd = sock_desc;
    }
}

void add_client_connection(int listener, fd_set *read_fs, int *max_sd, struct sockaddr_in address, int addrlen, struct client *client_socket)
{
    // accept new connection
    int new_socket;
    new_socket = accept(listener, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    printf("adding client . . .\n");

    // add new client sockcet to first opening
    int i;
    for (i = 0; i < MAXCONNECTIONS; i++)
    {
        if( client_socket[i].socket == 0 )
        {
            client_socket[i].socket = new_socket;
            client_socket[i].receive_private = false;
            printf("Adding ...\n");
            break;
        }
    }

    welcome_sequence(listener, read_fs, max_sd, client_socket, new_socket);
}


void set_socket_options(struct sockaddr_in *address)
{
    address->sin_family = FAMILY;
    address->sin_addr.s_addr = ADDRESS;
    address->sin_port = htons(PORT);
}


