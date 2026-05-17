#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<arpa/inet.h>

// socket
// bind
// listen
// accept

int main(){
	int cont, create_socket, new_socket, fd;
	socklen_t addrlen;
	int buffsize = 1024;
	char *buffer = malloc(buffsize);
	char fname[256];
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

	if (recv(new_socket, fname, sizeof(fname) - 1, 0) <= 0) {
		perror("recv filename");
		close(new_socket);
		close(create_socket);
		return 1;
	}

	fname[sizeof(fname) - 1] = '\0';

	if ((fd = open(fname, O_RDONLY)) < 0) {
		perror("open");
		close(new_socket);
		close(create_socket);
		return 1;
	}

	while ((cont = read(fd, buffer, buffsize)) > 0) {
		ssize_t w = write(1, buffer, cont);
		(void)w;
		if (send(new_socket, buffer, cont, 0) < 0) {
			perror("send");
			break;
		}
	}

	close(fd);
	close(new_socket);
	free(buffer);

	return close(create_socket);
}