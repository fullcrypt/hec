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

#define SIZE 64128 //size of encrypted int32
#define SERV_PORT 3000

FHEW::EvalKey *EK;

/* SIGCHLD handling.
   Waitpid instead of wait to avoid zombie processes */
void sig_child(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("\nchild %d terminated\n", pid);
    }
}

/* make it gomomorphic */
void GOMOMORPHIC_OPERATION(char* encrypted_data, char* modified_data, ssize_t N)
{
    /*int num = atoi(encrypted_data);
    printf("NUM = %d\n", num);
    ++num;
    snprintf(modified_data, SIZE, "%d", num);
    printf("%s\n", modified_data);*/
}

/* client processing */
void client_handle(int client_fd)
{
	EK = (FHEW::EvalKey *)malloc(EVAL_SIZE);
	
	read_data(client_fd, EK, EVAL_SIZE);

	/* Make gomomorphic operation using eval key received from the client  */
    char encrypted_data[SIZE];
    char modified_data[SIZE];
    ssize_t bytes_to_send = 0;

    printf("got file\n");
    if ((bytes_to_send = read_data(client_fd, encrypted_data, SIZE)) < 0) { //connection closed on client side
        err("reading");
        return;
    }
    
    printf("Got information from client. Making gomomorphic operation....");
    GOMOMORPHIC_OPERATION(encrypted_data, modified_data, bytes_to_send);
    printf("Done\n");

    ssize_t written_bytes = 0;
    if ((written_bytes = write_data(client_fd, encrypted_data, bytes_to_send)) < 0) {
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
	int optval = 1;

    /* connection prepare */
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
		perror("setsockopt:");
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
        
