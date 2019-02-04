
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
#include "config.h"
#include "globals.h"

void broadcast_new_message(int sock_desc, char *buffer, int max_sd, struct client *client_socket, fd_set *read_fd)
{
    // broadcast message to all connections
    int i;
    for(i = 0; i < max_sd; i++)
        {
        sock_desc = client_socket[i].socket;
            // Dont send to sender
            if(!FD_ISSET(sock_desc, read_fd))
            {
                send(sock_desc , buffer , strlen(buffer) , 0 );
            }
        }
}

void send_welcome_message(int new_socket)
{
    //send new connection greeting message and ask for username
    char *welcome_message = "Welcome! Please enter a username: ";
    send(new_socket, welcome_message, strlen(welcome_message), 0);
    puts("Welcome message sent successfully");
}

void send_private_message(char *buffer, int max_sd, struct client *client_socket, fd_set *read_fd, char *sender)
{
    // broadcast message to all connections
    printf("\nsend_private...1:\n");
    int sock_desc;
    char username[USERNAMESIZE];
    int i;
    for(i = 0; i < max_sd; i++)
        {
            sock_desc = client_socket[i].socket;
            strcpy(username, client_socket[i].username);

            if ((client_socket[i].receive_private == true))
            {
                printf("\nsend_private...2\n");
                // append_username_to_private_message(buffer, username);
                append_username_to_private_message(buffer, sender);
                send(sock_desc, buffer, strlen(buffer), 0);
                client_socket[i].receive_private = false;
            }
        }
    printf("\nsend_private...3:\n");
}

int is_private_message(char *buffer)
{
    if (buffer[0] == '@')
        return true;
    else
        return false;
}

void append_username_to_private_message(char *buffer, char *sender)
{
    // prepare to add "@" before name and ": " after name
    char *new_chars_1 = "[";
    char *new_chars_2 = "]: ";
    // for removing '@[receiver name]'
    int first_delimiter;
    first_delimiter = strcspn(buffer, " ");

    // add username and new chars to beginning of buffer
    char *temp = strdup(buffer);
    strcpy(buffer, new_chars_1);
    strcat(buffer, sender);
    strcat(buffer, new_chars_2);
    // chop of @[receiver name] from buffer
    strcat(buffer, &temp[first_delimiter]);

    //free the memory
    // free(temp);
    // free(new_chars);
}

void append_username_to_message(char *buffer, char *name)
{
    // prepare to add ": " after name
    char *new_chars = ": ";

    // add username and new chars to beginning of buffer
    char *temp = strdup(buffer);
    strcpy(buffer, name);
    strcat(buffer, new_chars);
    strcat(buffer, temp);

    //free the memory
    // free(temp);
    // free(new_chars);
}

void find_private_message_receiver(char *receiver, char *buffer, int max_sd, struct client *client_socket)
{
    printf("\nfind_private...1:");
    // Find location of first space
    int first_delimiter;
    first_delimiter = strcspn(buffer, " ");

    // extract reciever name
    memcpy(receiver, &buffer[1], first_delimiter);
    receiver[first_delimiter-1] = '\0';
    printf("\nfind_private...2:|%s|", receiver);

    int i;
    for(i = 0; i < max_sd; i++)
    {
        if(strncmp(receiver, client_socket[i].username, strlen(receiver)) == 0)
            client_socket[i].receive_private = true;
    }
}


