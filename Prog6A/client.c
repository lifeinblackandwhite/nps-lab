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
        // Send actual message length
        send(sockfd, buffer, strlen(buffer), 0);

        int n = recv(sockfd, buffer, sizeof(buffer), 0);

        if (n > 0)
        {
            buffer[n] = '\0';

            printf("Server Reply: %s", buffer);
        }
    }

    printf("\nEOF\n");
}

int main(int argc, char *argv[])
{
    int create_socket;

    struct sockaddr_in address;

    if (argc != 2)
    {
        printf("Usage: ./client <IP Address>\n");
        return 1;
    }

    create_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (create_socket < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("The socket was created\n");

    address.sin_family = AF_INET;
    address.sin_port = htons(15001);

    inet_pton(AF_INET, argv[1], &address.sin_addr);

    if (connect(create_socket,
                (struct sockaddr *)&address,
                sizeof(address)) == 0)
    {
        printf("Connected to server %s\n", argv[1]);
    }

    str_cli(stdin, create_socket);

    close(create_socket);

    return 0;
}
