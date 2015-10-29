#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "common.h"
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 1024
#define SERV_PORT 3000


/* SIGCHLD handling.
   Waitpid instead of wait to avoid zombie processes */
void sig_child(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
}

int main()
{
    /* server struct */
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* connection prepare */
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0) {
        err("socket");
    }
    if (bind(serv_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0) {
        err("bind");
    }
    if (listen(serv_fd, 5) != 0) {
        err("listen");
    }
    
    /* Signal handler for SIGCHLD */
    signal(SIGCHLD, sig_child);

    /* server work */
    int client_fd = 0;
    pid_t child_pid = 0;
    struct sockaddr_in cliaddr;
    printf("Server is ready. Waiting for a clients...\n");
    while (1)
    {
        socklen_t len = sizeof(cliaddr);
        if ((client_fd = accept(serv_fd, (struct sockaddr*) &cliaddr, &len)) < 0) {
            if (errno == EINTR) { //signal interrupt
                continue;
            } else {
                err("accept");
            }
        }
        write(1, "new client\n", 11);
        if ((child_pid = fork()) == 0) { //child process
            close(serv_fd);
            //...client processing
#define MAX 256
            char buf[MAX];
            read(client_fd, buf, MAX);
            printf("%s\n", buf);
            exit(0);
        }
    }
    close(client_fd);
    return 0;
}
        
