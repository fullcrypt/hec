/**
** This program is a TCP-client, which can send(not yet) fully homomorphic encrypted data 
** to server for processing
**/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "net.h"

#define SIZE 1024
#define PORT 3000

/* write N bytes to descriptor serv_fd */
ssize_t send_data(int serv_fd, const void* ptr, size_t N)
{
    size_t bytes_num = N;
    ssize_t written_bytes = 0;
    const char* tmp_ptr = (char *) ptr;

    while (bytes_num > 0) {
        if ((written_bytes = write(serv_fd, tmp_ptr, bytes_num)) < 0) {
            if (errno == EINTR) { //signal interruption
                written_bytes = 0;
                continue;
            } else { //error occured
                return -1;
            }
        }
        bytes_num -= written_bytes;
        tmp_ptr += written_bytes;
    }
    return written_bytes;
}

int main(int argc, char** argv)
{
    /* data encrypting */
    //..
    void* encrypted_data = NULL;

    /* connection preparing */
    if (argc != 2) {
        help(argv[0]);
    }
    struct sockaddr_in servaddr; //server struct
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd < 0) {
        err("socket");
    }
    if (connect(serv_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        err("connect");
    }
    /* data sending */
    ssize_t written_bytes = 0;
    size_t bytes_to_send = sizeof(encrypted_data);
    if ((written_bytes = send_data(serv_fd, encrypted_data, bytes_to_send)) < 0) {
        err("sending data");
    } else if (written_bytes != bytes_to_send) {
        printf("%zd bytes were not sent!!! \n", bytes_to_send - written_bytes); 
        err("sending data");
    } else {
        printf("%zu was successfully sent to server!\n", bytes_to_send);
    }

    close(serv_fd);

    return 0;
}
        
