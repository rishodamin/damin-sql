#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define QUERY_SIZE 1000

void getCredentials(char *username, char *password, char *database) {
     // Get username
    printf("Enter username: ");
    fgets(username, MAX_LEN, stdin);
    username[strcspn(username, "\n")] = 0; 

    // Get password (hiding input can be done in system-dependent ways)
    printf("Enter password: ");
    fgets(password, MAX_LEN, stdin);
    password[strcspn(password, "\n")] = 0;

    // Get database name
    printf("Enter database name: ");
    fgets(database, MAX_LEN, stdin);
    database[strcspn(database, "\n")] = 0;
}

void executeQuery(const char *query, MYSQL *conn){
    MYSQL_RES *res;
    MYSQL_ROW row;
    if (mysql_query(conn, query)) {
        fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return;
    }
    
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        for(int i = 0; i < num_fields; i++) {
            printf("%s ", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    
    mysql_free_result(res);
}

int main() {
    char username[MAX_LEN];
    char password[MAX_LEN];
    char database[MAX_LEN];
    MYSQL *conn;
    char query[QUERY_SIZE];
    conn = mysql_init(NULL);

    getCredentials(username, password, database);

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, "localhost", username, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the Database successfully!\n");
    while (1)
    {
    printf(">> ");
        fgets(query, QUERY_SIZE, stdin);
        query[strcspn(query, "\n")] = 0; 

        if (strcmp(query, "exit()") == 0) {  
            printf("Exiting...\n");
            break;
        }

        executeQuery(query, conn);
    }
    mysql_close(conn);
    
    
}