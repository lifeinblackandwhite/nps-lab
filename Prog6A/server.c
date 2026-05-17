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

        printf("Client Message: %s", buffer);

        send(connfd, buffer, n, 0);
    }
}

int main()
{
    int listenfd, connfd, pid;
    socklen_t addrlen;

    struct sockaddr_in server_addr, client_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0)
    {
        printf("Socket creation failed\n");
        return 1;
    }

    printf("Socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(15001);

    if (bind(listenfd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        printf("Bind failed\n");
        return 1;
    }

    printf("Binding successful\n");

    listen(listenfd, 5);

    printf("Server is listening...\n");

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

        printf("Client %s connected on port %d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        pid = fork();

        if (pid == 0)
        {
            // Child process

            close(listenfd);

            printf("Inside child process\n");

            str_echo(connfd);

            close(connfd);

            exit(0);
        }

        // Parent process
        close(connfd);
    }

    close(listenfd);

    return 0;
}
