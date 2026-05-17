#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<string.h>

void remote_command(int connfd, int port){
	int n=1, buffsize=1024;
	char *buffer = malloc(buffsize);

	if (!buffer) return;

	do{
		while((n = recv(connfd, buffer, buffsize - 1, 0)) > 0){
			buffer[n] = '\0';
			if (send(connfd, buffer, n, 0) < 0)
				perror("send");

			printf("Port Number: %d\n", port);
#ifdef ENABLE_REMOTE_COMMAND_EXEC
			/* Only execute when explicitly enabled at build time */
			system(buffer);
#else
			/* Execution disabled: log the command instead */
			fprintf(stderr, "[remote_command] execution disabled. Command received: %s\n", buffer);
#endif
		}
	} while(n > 0);

	free(buffer);
}


int main(int argc, char const *argv[])
{
	int cont, create_socket, new_socket, addrlen, fd, pid;
	struct sockaddr_in address;

	if((create_socket  = socket(AF_INET, SOCK_STREAM, 0)) > 0)
		printf("The socket is created successfully\n");

	address.sin_family = AF_INET;
	address.sin_port = htons(15001);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(create_socket, (struct sockaddr*)&address, sizeof(address)) == 0)
		printf("Binding Successfull\n");

	listen(create_socket, 3);

	printf("The server is listening\n");

	while(1){
		addrlen = sizeof(struct sockaddr);
		if ((new_socket = accept(create_socket, (struct sockaddr*)&address, &addrlen)) > 0){
			printf("The client is connected: %s\n", inet_ntoa(address.sin_addr));
		}

		if((pid = fork()) == 0){
			printf("Inside Child\n");
			close(create_socket);
			remote_command(new_socket, htons(address.sin_port));
			exit(0);
		}
		close(new_socket);
	}
	return 0;
}
