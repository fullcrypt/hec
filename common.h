#include <stdlib.h>


/*
** Common functions/types for both client and server
*/

void help(char* prog)
{
    printf("Usage: %s <ip_address>\n", prog);
    exit(0);
}

/* dummy error processing */
void err(const char* err_name)
{
    printf("Error occured while %s\n", err_name);
    exit(EXIT_FAILURE);
}



