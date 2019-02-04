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

#include "config.h"
#include "globals.h"


void add_client_username(fd_set *read_fs, struct client *client_socket, char *buf)
{

    int sock_desc;
    // Incoming transmission
    // Loop through all connections
    int i;
    for (i=0; i < MAXCONNECTIONS; i++)
    {
        sock_desc = client_socket[i].socket;
        // if a connected client triggered read
        if (FD_ISSET(sock_desc, read_fs))
        {
            // associate username with client
            strcpy(client_socket[i].username, buf);
        }
    }
}

void enter_new_user_password(int listener, fd_set *read_fs, int *max_sd, struct client *client_socket, int new_socket, char *password)
{
    char *new_password_message = "Username not found. Please enter a password for the new user: ";
    send(new_socket, new_password_message, strlen(new_password_message), 0);

    //wait for password
    wait_for_input(listener, read_fs, max_sd, client_socket);

    //if input comes from client socket
    if (!FD_ISSET(listener, read_fs))
    {
        read_into_buffer(new_socket, password);
    }

}
