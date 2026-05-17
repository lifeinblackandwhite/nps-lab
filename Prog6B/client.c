#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void str_cli(FILE *fp, int sockfd)
{
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        send(sockfd, buffer, strlen(buffer), 0);

        int n = recv(sockfd, buffer, sizeof(buffer), 0);

        if (n > 0)
        {
            buffer[n] = '\0';

            printf("Server Reply: %s", buffer);
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd;

    struct sockaddr_in address;

    if (argc != 2)
    {
        printf("Usage: ./client <IP Address>\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("Socket created successfully\n");

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);

    inet_pton(AF_INET, argv[1], &address.sin_addr);

    if (connect(sockfd,
                (struct sockaddr *)&address,
                sizeof(address)) < 0)
    {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server %s\n", argv[1]);

    str_cli(stdin, sockfd);

    close(sockfd);

    return 0;
}
