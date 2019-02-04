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

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}


void connect_to_database(MYSQL *con)
{
  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "tester", "tester_pw",
          "my_server", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }
}

void save_new_user_credentials(char *name, char *password)
{
    //cosntruct query
    char insert_query[1000];
    insert_query[0] = '\0';
    strcat(insert_query, "INSERT INTO user_credentials(username, password) VALUES('");
    strcat(insert_query, name);
    strcat(insert_query, "', '");
    strcat(insert_query, password);
    strcat(insert_query, "')");

    if (mysql_query(con, insert_query))
    {
      finish_with_error(con);
    }
    puts("New user added!\n");
}

my_boolean is_new_user(char *name)
{
    //cosntruct query
    char query[BUFFERSIZE];
    query[0] = '\0';
    strcat(query, "SELECT username FROM user_credentials WHERE username='");
    strcat(query, name);
    strcat(query, "'");

    //run query and store results
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    // check if empty set
    int num_of_results = mysql_num_rows(result);
    //
    if(num_of_results < 1)
        return true;
    else
        return false;
}
