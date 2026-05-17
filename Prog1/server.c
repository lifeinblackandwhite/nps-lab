
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
	int create_socket, new_socket;
	socklen_t addrlen;
	int buffsize = 1024;
	char buffer[1024];
	struct sockaddr_in address;

	if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(15001);
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind");
		close(create_socket);
		return 1;
	}

	if (listen(create_socket, 3) < 0) {
		perror("listen");
		close(create_socket);
		return 1;
	}

	addrlen = sizeof(struct sockaddr_in);

	if ((new_socket = accept(create_socket, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		close(create_socket);
		return 1;
	}

	printf("Client connected: %s\n", inet_ntoa(address.sin_addr));

	while (1) {
		// Receive message from client
		ssize_t recv_len = recv(new_socket, buffer, buffsize - 1, 0);
		if (recv_len <= 0) {
			printf("Client disconnected or error.\n");
			break;
		}
		buffer[recv_len] = '\0';
		printf("Client: %s\n", buffer);
		if (strcmp(buffer, "exit") == 0) {
			printf("Conversation ended by client.\n");
			break;
		}

		// Send message to client
		printf("Server: ");
		fflush(stdout);
		if (!fgets(buffer, buffsize, stdin)) break;
		buffer[strcspn(buffer, "\n")] = 0;
		if (send(new_socket, buffer, strlen(buffer), 0) < 0) {
			perror("send");
			break;
		}
		if (strcmp(buffer, "exit") == 0) {
			printf("Conversation ended by server.\n");
			break;
		}
	}

	close(new_socket);
	close(create_socket);
	return 0;
}