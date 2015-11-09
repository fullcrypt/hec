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
#include <FHEW.h>
#include <LWE.h>
#include <iostream>
#include <assert.h>
#define SIZE 64128
#define PORT 3000


LWE::SecretKey LWEsk;
/* Make it gomomorphic */
char* FULLY_GOMOMORPHIC_ENCRYPT_DATA(char* data)
{
    printf("Setting up FHEW \n");
    FHEW::Setup();
    printf("Generating secret key ... ");
    LWE::KeyGen(LWEsk);
    printf(" Done.\n");
   
    /* encrypting */
    unsigned int message = atoll(data);
    int two_pow_31 = 1 << 31;
    int bit;
    LWE::CipherText* ct = new LWE::CipherText[32];
    printf("Binary: ");
    for (int i = 0; i < 32; i++) {
        bit = (message & two_pow_31) ? 1 : 0;
        printf("%d", bit);
        LWE::Encrypt(&ct[i], LWEsk, bit);
        message = message << 1;
    }
    printf("\n");

    return (char*)ct;
}

void get_answer(int fd, char* answer) 
{
    unsigned get_bytes = 0;
    if ((get_bytes = read(fd, answer, SIZE)) < 0) { //connection closed by server
        err("reading answer");
    }
    printf("%u bytes was received from server\n", get_bytes);
    unsigned int m = 0;
    int temp;
    LWE::CipherText* ct = (LWE::CipherText*) answer;
    for (int i = 0; i < 32; i++)
    {
        temp = LWE::Decrypt(LWEsk, ct[i]);
        m += temp;
        if (i != 31) {
            m = m << 1;
        }
    }
    printf("\nGot from server: decimal = %d\n", m);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        help(argv[0]);
    }

    /* data encrypting */
    char data[SIZE];
    printf("Put number = ");
    scanf("%s", data);
    char* encrypted_data = FULLY_GOMOMORPHIC_ENCRYPT_DATA(data);

    /* connection preparing */
    struct sockaddr_in servaddr; //server struct
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    /* connecting to server */
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(serv_fd, &servaddr, sizeof(servaddr));
    
    /* encrypted data sending */
    ssize_t written_bytes = 0;
    ssize_t bytes_to_send = sizeof(data);
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
    get_answer(serv_fd, encrypted_result);
    close(serv_fd);

    return 0;
}
        
