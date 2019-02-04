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

#include "globals.h"

#include "config.h"


void welcome_sequence(int listener, fd_set *read_fs, int *max_sd, struct client *client_socket, int new_socket)
{
    send_welcome_message(new_socket);
    char name[USERNAMESIZE];
    char pw[BUFFERSIZE];

    //wait for username
    wait_for_input(listener, read_fs, max_sd, client_socket);

    //if input comes from client socket
    if (!FD_ISSET(listener, read_fs))
    {
        read_into_buffer(new_socket, name);
        if(is_new_user(name))
        {
            enter_new_user_password(listener, read_fs, max_sd, client_socket, new_socket, pw);
            save_new_user_credentials(name, pw);
        }
            add_client_username(read_fs, client_socket, name);
    }
}

void read_into_buffer(int sock_desc, char *buffer)
{
    int read_len;
    read_len = read(sock_desc, buffer, BUFFERSIZE);
    // Add terminating character to end
    buffer[read_len] = '\0';

}

void read_incoming_message(char *buffer, int max_sd, struct client *client_socket, fd_set *read_fd)
{
    printf("read_incoming...");
    char receiver[USERNAMESIZE];
    char sender[USERNAMESIZE];
    int sock_desc;
    // Incoming transmission
    // Loop through all connections
    int i;
    for (i=0; i < MAXCONNECTIONS; i++)
    {
        sock_desc = client_socket[i].socket;
        // if a connected client triggered read
        if (FD_ISSET(sock_desc, read_fd))
        {
            read_into_buffer(sock_desc, buffer);
            if (is_private_message(buffer))
            {
                strcpy(sender, client_socket[i].username);
                find_private_message_receiver(receiver, buffer, max_sd, client_socket);
                send_private_message(buffer, max_sd, client_socket, read_fd, sender);
            }
            else
            {
                append_username_to_message(buffer, client_socket[i].username);
                broadcast_new_message(sock_desc, buffer, max_sd, client_socket, read_fd);
            }


        }
    }
}

int wait_for_input(int listener, fd_set *read_fd, int *max_sd, struct client *client_socket)
{
        // prepare fd sets (just read in this case)
        add_listener_set(listener, read_fd);
        *max_sd = listener;
        add_connection_sets(client_socket, read_fd, max_sd);

        // Wait for activity indefinitely
        int activity;
        activity = select( *max_sd + 1 , read_fd , NULL , NULL , NULL);
        return activity;
}
