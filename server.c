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

/* make it gomomorphic */
void GOMOMORPHIC_OPERATION(char* encrypted_data, char* modified_data)
{
    int num = atoi(encrypted_data);
    printf("NUM = %d\n", num);
    ++num;
    snprintf(modified_data, SIZE, "%d", num);
    printf("%s\n", modified_data);
}

void client_handle(int client_fd)
{
    //...client processing
    char encrypted_data[SIZE];
    char modified_data[SIZE];
    if (read(client_fd, encrypted_data, SIZE) < 0) { //connection closed on client side
        err("reading");
        return;
    }
    printf("Get information from client. Making gomomorphic operation....");
    GOMOMORPHIC_OPERATION(encrypted_data, modified_data);
    printf("Done\n");

    ssize_t written_bytes = 0;
    size_t bytes_to_send = strlen(modified_data)+1;
    if ((written_bytes = send_data(client_fd, modified_data, bytes_to_send)) < 0) {
        err("sending data");
    } else if (written_bytes != bytes_to_send) {
        printf("%zd bytes were not sent!!! \n", bytes_to_send - written_bytes); 
        err("sending data");
    } else {
        printf("%zu bytes was successfully sent to client!\n", bytes_to_send);
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
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(serv_fd, &servaddr, sizeof(servaddr));
    Listen(serv_fd, 5); //5 = backlog 
    
    /* Signal handler for SIGCHLD && SIGPIPE */
    signal(SIGCHLD, sig_child);
    signal(SIGPIPE, SIG_IGN);
    
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
        if ((child_pid = fork()) == 0) { //child process
            close(serv_fd);
            client_handle(client_fd);
            exit(0);
        }
    }
    close(client_fd);
    return 0;
}
        
