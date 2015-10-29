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
#include "common.h"

#define SIZE 1024
#define PORT 3000


/* Make it gomomorphic */
char* FULLY_GOMOMORPHIC_ENCRYPT_DATA(char* data)
{
    return data;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        help(argv[0]);
    }

    /* data encrypting */
    char data[SIZE];
    scanf("%s", data);
    char* encrypted_data = eFULLY_GOMOMORPHIC_ENCRYPT_DATA(data);
    
    /* connection preparing */
    struct sockaddr_in servaddr; //server struct
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    /* connecting to server */
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(serv_fd, &servaddr, sizeof(servaddr));
    
    /* encrypting data sending */
    ssize_t written_bytes = 0;
    size_t bytes_to_send = strlen(encrypted_data)+1;
    if ((written_bytes = send_data(serv_fd, encrypted_data, bytes_to_send)) < 0) {
        err("sending data");
    } else if (written_bytes != bytes_to_send) {
        printf("%zd bytes were not sent!!! \n", bytes_to_send - written_bytes); 
        err("sending data");
    } else {
        printf("%zu bytes was successfully sent to server!\n", bytes_to_send);
    }
    
    /* get answer from server*/
    char encrypted_result[SIZE];
    if (read(serv_fd, encrypted_result, SIZE) < 0) { //connection closed by server
        err("reading answer");
    }
    printf("get:%s\n", encrypted_result);
    close(serv_fd);

    return 0;
}
        
