#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void str_echo(int connfd)
{
    int n;
    char buffer[1024];

    while ((n = recv(connfd, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[n] = '\0';

        // Print message on server terminal
        printf("Client Message: %s", buffer);

        // Send message back to client
        send(connfd, buffer, n, 0);
    }
}

int main()
{
    int listenfd, connfd;
    socklen_t addrlen;

    struct sockaddr_in address, client_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("Socket created successfully\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(15001);

    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Bind failed\n");
        return 1;
    }

    printf("Binding successful\n");

    listen(listenfd, 5);

    printf("Server is listening on port 15001...\n");

    while (1)
    {
        addrlen = sizeof(client_addr);

        connfd = accept(listenfd,
                        (struct sockaddr *)&client_addr,
                        &addrlen);

        if (connfd < 0)
        {
            printf("Accept failed\n");
            continue;
        }

        printf("Client connected: %s : %d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        str_echo(connfd);

        close(connfd);
    }

    close(listenfd);

    return 0;
}
