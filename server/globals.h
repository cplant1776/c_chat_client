#include <my_global.h>
#include <mysql.h>

#include "config.h"

#ifndef MY_BOOLEAN
#define MY_BOOLEAN

typedef enum { false, true } my_boolean;

#endif // MY_BOOLEAN

#ifndef CLIENT
#define CLIENT
struct client
{
    int socket;
    char username[USERNAMESIZE];
    my_boolean receive_private;
};
#endif // CLIENT

#ifndef DATABASECONNECTION
#define DATABASECONNECTION
//global database conneciton
MYSQL *con;
#endif //DATABASECONNECTION
