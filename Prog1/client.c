#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <server-ip>\n", argv[0]);
		return 1;
	}

	int create_socket;
	int buffsize = 1024;
	char buffer[1024];
	struct sockaddr_in address;

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(15001);
	if (inet_pton(AF_INET, argv[1], &address.sin_addr) != 1) {
		fprintf(stderr, "Invalid address: %s\n", argv[1]);
		return 1;
	}

	if (connect(create_socket, (struct sockaddr *)&address, sizeof(address)) != 0) {
		perror("connect");
		close(create_socket);
		return 1;
	}

	while (1) {
		// Send message to server
		printf("Client: ");
		fflush(stdout);
		if (!fgets(buffer, buffsize, stdin)) break;
		buffer[strcspn(buffer, "\n")] = 0;
		if (send(create_socket, buffer, strlen(buffer), 0) < 0) {
			perror("send");
			break;
		}
		if (strcmp(buffer, "exit") == 0) {
			printf("Conversation ended by client.\n");
			break;
		}

		// Receive message from server
		ssize_t recv_len = recv(create_socket, buffer, buffsize - 1, 0);
		if (recv_len <= 0) {
			printf("Server disconnected or error.\n");
			break;
		}
		buffer[recv_len] = '\0';
		printf("Server: %s\n", buffer);
		if (strcmp(buffer, "exit") == 0) {
			printf("Conversation ended by server.\n");
			break;
		}
	}

	close(create_socket);
	return 0;
}