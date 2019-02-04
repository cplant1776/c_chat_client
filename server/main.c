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
#include "database.h"
#include "chatting.h"
#include "file_descriptors.h"
#include "users.h"
#include "server.h"
#include "globals.h"

#include "config.h"



int main(int argc , char *argv[])
{
    int listener;
    int addrlen, max_sd;
    struct sockaddr_in address;
    char buffer[BUFFERSIZE];
    struct client client_socket[MAXCONNECTIONS];

    // Open global database connection
    con = mysql_init(NULL);
    connect_to_database(con);

    // int client_socket[MAXCONNECTIONS];
    fd_set read_fd;

    //initialise all client_socket[] to 0 so not checked
    memset(client_socket, 0, sizeof client_socket);

    // create a listening socket
    listener = socket(AF_INET , SOCK_STREAM , 0);

    // listener options (set by macros at top of file)
    set_socket_options(&address);

    // bind listener to PORT in macro
    bind(listener, (struct sockaddr *)&address, sizeof(address));

    // start listening to no more than MAXCONNECTIONS connections
    listen(listener, MAXCONNECTIONS);

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("running . . .");

    while(1)
    {
        printf("loop\n");
        // Wait for activity indefinitely
        wait_for_input(listener, &read_fd, &max_sd, client_socket);

        if (FD_ISSET(listener, &read_fd))
        {
            add_client_connection(listener, &read_fd, &max_sd, address, addrlen, client_socket);
        }

        // Iotherwise its a connected socket
        read_incoming_message(buffer, max_sd, client_socket, &read_fd);
    }

    return 0;
}
