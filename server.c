#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024
#define SERV_PORT 3000

int main()
{
    /* server struct */
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* socker prepare */
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

    /* server work */
    struct sockaddr_in cliaddr;
    char buf[SIZE];
    printf("Server is ready. Waiting for a clients...\n");
    /*while (1)
    {
        socklen_t len = sizeof(cliaddr);
        int client_fd = accept(serv_fd, (struct sockaddr*) &cliaddr, &len);
        int n = read(client_fd, buf, SIZE);
        if (n < 0) {
            err("read");
        }
        if (write(client_fd, buf, strlen(buf)) < 0) {
            err("write");
        }
        close(client_fd);
    }*/
    return 0;
}
        
