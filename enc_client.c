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
#include "./FHEW-extensions/libfhew-extensions/FHEW-extensions.h"
#define SIZE 64128
#define PORT 3000

LWE::SecretKey *LWEsk = NULL;
FHEW::EvalKey *EK = NULL;
char filename[255] = {'\0'};
LWE::CipherText ct[32];


void read_secret_from_file(char *filename)
{
	int fd;
	int current = 0;
	int number_read;

	fd = open(filename, O_RDONLY);

	while (current != sizeof(LWE::SecretKey)) {
		number_read = read(fd, *LWEsk + current, sizeof(LWE::SecretKey) - current);
		current += number_read;
	}

	close(fd);
}

void generate_eval()
{
	printf("Enter filename for secret key:\n");
	scanf("%s", filename);

	printf("Generating eval key\n");
    FHEW::KeyGen(EK, *LWEsk);

	SaveEvalKey(EK, filename);
}

void send_vote(int vote, int serv_fd)
{
    int two_pow_32 = 1 << 31;
    int bit;
   
    for (int i = 0; i < 32; i++) {
        bit = (vote & two_pow_32) ? 1 : 0;
        LWE::Encrypt(&ct[i], *LWEsk, bit);
        vote = vote << 1;
    }

    write_data(serv_fd, ct, sizeof(LWE::CipherText) * 32);
}

void parse_results(LWE::CipherText *ct)
{
    unsigned int results = 0;
    int temp;

    for (int i = 0; i < 32; i++) {
        temp = LWE::Decrypt(*LWEsk, ct[i]);
        results += temp;
        if (i != 31)
            results = results << 1;
    }

    printf("Results:\n"
            "Candidate a: %d\n"
            "Candidate b: %d\n"
            "Candidate c: %d\n"
            "Candidate d: %d\n",
            (results & CANDIDATE_A_MASK) >> CANDIDATE_A_SHIFT,
            (results & CANDIDATE_B_MASK) >> CANDIDATE_B_SHIFT,
            (results & CANDIDATE_C_MASK) >> CANDIDATE_C_SHIFT,
            (results & CANDIDATE_D_MASK) >> CANDIDATE_D_SHIFT);

}

int main(int argc, char** argv)
{
	int type;   
	struct client_request req;
	int need_to_send_eval;
	int vote;
	char c;
	int secret_fd;
    LWE::CipherText answer_from_server[32];

	if (argc != 3 && argc != 4) {
		help(argv[0]);
	}

    /* Send you dreams to server */
    char data[SIZE];

    /* connection preparing */
    struct sockaddr_in servaddr; //server struct
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	printf("Connecting to server...\n");

    /* connecting to server */
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(serv_fd, &servaddr, sizeof(servaddr));

	FHEW::Setup();

	printf("Connected!!\nReading secret key\n"); 

    LWEsk = LoadSecretKey(argv[2]);

	if (argc == 4) {
		printf("Reading eval key...\n");
        EK = LoadEvalKey(argv[3]);
	} else {
        printf("Generating eval key...\n");
        FHEW::KeyGen(EK, *LWEsk);
	}
	printf("Done.\n\n");

	printf("What do you want???\n"
		    "Vote[0], See results[1]:\n");
    scanf("%d", &type);
	if (type != 1 && type != 0) {
		printf("Wrong input\n");
		return 0;
	}
	switch(type) {
	/* Vote */
	case 0:
		printf("To make your vote write 'a', 'b', 'c' or 'd':\n");
		getchar();
        c = getchar();
		switch(c) {
		case 'a':
			vote = CANDIDATE_A_VOTE;
			break;
		case 'b':
			vote = CANDIDATE_B_VOTE;
			break;
		case 'c':
			vote = CANDIDATE_C_VOTE;
			break;
		case 'd':
			vote = CANDIDATE_D_VOTE;
			break;
		default:
			printf("Wrong input\n");
			return 0;
		}

		req.type = CLIENT_REQUEST_VOTE;
		write_data(serv_fd, &req, sizeof(struct client_request));
		read_data(serv_fd, &need_to_send_eval, sizeof(int));
		if (need_to_send_eval) {
            printf("Sending eval key...\n");
			send_file((argc == 4) ? argv[3] : filename, serv_fd);
		}
        printf("Sending vote...\n");
		send_vote(vote, serv_fd);
        printf("Done.\n");
        break;
	/* Results */
	case 1:
		req.type = CLIENT_REQUEST_SEE_RESULTS;
		write_data(serv_fd, &req, sizeof(struct client_request));
		read_data(serv_fd, &answer_from_server, sizeof(LWE::CipherText) * 32);
        parse_results(answer_from_server);
	}

    return 0;
}
        
