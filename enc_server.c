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
#include <pthread.h>
#include "./FHEW-extensions/libfhew-extensions/FHEW-extensions.h"

#define SIZE 64128 //size of encrypted int32
#define SERV_PORT 3000

pthread_mutex_t results_lock;
pthread_mutex_t eval_lock;

bool first_client = true;

FHEW::EvalKey *EK = NULL;
LWE::CipherText results[32];

char *filename = "eval.txt_server";

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

/* client processing */
void *client_handle(void *arg)
{
    unsigned long long __client_fd = (unsigned long long)arg;
	int client_fd = __client_fd;
	int answer;
    LWE::CipherText ct[32];
    LWE::CipherText tmp[32];
    LWE::CipherText *carry_out;
    struct client_request req;

	pthread_mutex_lock(&eval_lock);
	if (!EK) {
		EK = (FHEW::EvalKey *)malloc(EVAL_SIZE);
		if (!EK) {
			perror("malloc evalkey:");
		}
	}
	pthread_mutex_unlock(&eval_lock);
	
	read_data(client_fd, &req, sizeof(struct client_request));

	switch (req.type) {
	case CLIENT_REQUEST_SEE_RESULTS:
		pthread_mutex_lock(&results_lock);
		write_data(client_fd, &results, sizeof(LWE::CipherText) * 32);
		pthread_mutex_unlock(&results_lock);
		break;
	case CLIENT_REQUEST_VOTE:
		pthread_mutex_lock(&eval_lock);
		answer = (EK == NULL) ? 1 : 0;
		write_data(client_fd, &answer, sizeof(int));

		if (answer) {
			get_file(client_fd, filename);
            EK = LoadEvalKey(filename);
		}
		pthread_mutex_unlock(&eval_lock);

        read_data(client_fd, ct, sizeof(LWE::CipherText) * 32);
        pthread_mutex_lock(&results_lock);
        if (first_client) {
            memcpy(results, ct, sizeof(LWE::CipherText) * 32);
            first_client = 0;
        } else {
            carry_out = new LWE::CipherText;
            memcpy(tmp, results, sizeof(LWE::CipherText) * 32);
            HomSUM32(results, carry_out, *EK, ct, tmp);
        }
        pthread_mutex_unlock(&results_lock);
	}
}

int main(int argc, char **argv)
{
    FHEW::Setup();
	if (argc == 2) {
        printf("Reading eval key...\n");
        EK = LoadEvalKey(argv[1]);
	}

    long long __client_fd;

    /* server struct */
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int optval = 1;
	pthread_t pthread = {0};

	pthread_mutex_init(&results_lock, NULL);
	pthread_mutex_init(&eval_lock, NULL);

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

        memset(&pthread, 0, sizeof(pthread_t));
        __client_fd = client_fd;
		pthread_create(&pthread, NULL, client_handle, (void *)__client_fd);
    }
    close(client_fd);
    return 0;
}
