#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <FHEW.h>
#include <LWE.h>

#define EVAL_SIZE	n * (BS_base - 1) * BS_exp * sizeof(FHEW::ct_FFT) + \
					N * KS_base * KS_exp * sizeof(LWE::CipherTextQ)

/*
** Common functions/types for both client and server
*/

void help(char* prog)
{
    printf("Usage: %s <ip_address> <eval_file>\n", prog);
    exit(0);
}

/* dummy error processing */
void err(const char* err_name)
{
    printf("Error occured while %s\n", err_name);
    exit(EXIT_FAILURE);
}

int Socket(int domain, int type, int protocol)
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        err("socket");
    }
    return sock_fd;
}

void Connect(int serv_fd, struct sockaddr_in* servaddr, size_t size)
{
    if (connect(serv_fd, (struct sockaddr*) servaddr, size) < 0) {
        err("connect");
    }
}

void Bind(int serv_fd, struct sockaddr_in* servaddr, size_t size)
{
    if (bind(serv_fd, (struct sockaddr*) servaddr, size) != 0) {
        err("bind");
    }
}

void Listen(int serv_fd, int backlog)
{
    if (listen(serv_fd, backlog) != 0) {
        err("listen");
    }
}

/* write N bytes to descriptor fd */
ssize_t write_data(int fd, const void* ptr, size_t N)
{
    size_t bytes_num = N;
    ssize_t written_bytes = 0;
    const char* tmp_ptr = (char *) ptr;

    while (bytes_num > 0) {
        if ((written_bytes = write(fd, tmp_ptr, bytes_num)) < 0) {
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

/*
 * read N bytes from descriptor fd 
 * you can call this function
 */
ssize_t read_data(int fd, void* ptr, size_t N)
{
    size_t  bytes_num = N; // bytes left to read
    ssize_t read_num = 0;  // already read bytes
    char* tmp_ptr = (char *) ptr;

    while (bytes_num > 0) {
        if ((read_num = read(fd, tmp_ptr, bytes_num)) < 0) {
            if (errno == EINTR) {
                read_num = 0;
                continue;
            }
            else { //error occured
                return -1;
            }
        } else if (read_num == 0) {
            break; //EOF
        }
        bytes_num -= read_num;
        tmp_ptr += read_num;
    }
    return N - bytes_num; //return N if all bytes were read
}

/* sending file */
void send_file(const char* file, int serv_fd)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        err("with file");
    }
    char buf[1024];
    ssize_t send_portion = 0;
    ssize_t read_portion = sizeof(buf);
    while (1) {
        fflush(stdin);
        send_portion = read_data(fd, buf, read_portion);
        if (!send_portion) {
            break;
        }
        if ((write_data(serv_fd, buf, send_portion)) < 0) {
            err("send_file:write_data");
        }
    }
    close(fd);
}

/* get file (brilliant comment from team lead)*/
/*
 * client _must_ send 8 bytes with filesize before file to
 * avoid infinite loop.
 */

void get_file(int client_fd, char *eval_file)
{
    int eval_fd = open(eval_file, O_CREAT | O_WRONLY, 0644);
    char buf[1024];
	int filesize;
    int read_portion = sizeof(buf);
    int write_portion = 0;

	read(client_fd, &filesize, sizeof(int));

    while (filesize) {
        write_portion = read_data(client_fd, buf, (filesize > read_portion) ? read_portion: filesize);
		printf("filesize = %d\n", filesize);
		fflush(stdout);
		filesize -= write_portion;
        if (!write_portion) {
            break;
        }
        printf("write portion = %d\n", write_portion);
        if (write_data(eval_fd, buf, write_portion) < 0) {
            err("write eval key");
        }
    }
    close(eval_fd);
}
