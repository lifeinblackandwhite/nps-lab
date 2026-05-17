#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/socket.h>
#include<string.h>

void str_cli(FILE *fp, int sockfd){
	int cont, buffsize = 1024;
	char *buffer = malloc(buffsize);

	while (fgets(buffer, buffsize, fp) != NULL) {
		size_t tosend = strlen(buffer);
		if (send(sockfd, buffer, tosend, 0) < 0)
			perror("send");

		cont = recv(sockfd, buffer, buffsize - 1, 0);
		if (cont > 0) {
			buffer[cont] = '\0';
			fputs(buffer, stdout);
		}
	}
}

int main(int argc, char *argv[])
{
	int create_socket, cont;
	int buffsize = 1024;
	char *buffer = malloc(buffsize);
	char fname[256];
	struct sockaddr_in address;

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]);
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(15001);
	if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1) {
		fprintf(stderr, "Invalid address: %s\n", argv[1]);
		return 1;
	}

	if ((connect(create_socket, (struct sockaddr *)&address, sizeof(address))) != 0) {
		perror("connect");
		close(create_socket);
		return 1;
	}

	str_cli(stdin, create_socket);
	free(buffer);
	return close(create_socket);
}
