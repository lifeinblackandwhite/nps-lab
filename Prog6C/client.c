#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>


void str_cli(FILE *fp, int sockfd, struct sockaddr *serv_address, int servlen)
{
    int bufsize = 1024, cont;
    char *buffer = malloc(bufsize);
    int addrlen = sizeof(struct sockaddr_in);
    while (fgets(buffer, bufsize, fp) != NULL)
    {
        size_t tosend = strlen(buffer);
        sendto(sockfd, buffer, tosend, 0, serv_address, servlen);
        if ((cont = recvfrom(sockfd, buffer, bufsize - 1, 0, NULL, NULL)) > 0)
        {
            buffer[cont] = '\0';
            fputs(buffer, stdout); // ECHO PRINTING..
        }
    }
    printf("\nEOF\n");
}


int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_address;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]);
        return 1;
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return 1;
    }

    printf("The Socket was created\n");
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(16001);
    if (inet_pton(AF_INET, argv[1], &serv_address.sin_addr) != 1) {
        fprintf(stderr, "Invalid address: %s\n", argv[1]);
        return 1;
    }

    str_cli(stdin, sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address));

    exit(0);
}