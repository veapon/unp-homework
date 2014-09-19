#include "server.h"

#define TRUE 1
#define FALSE 0

void request_handler(int);

int main(int argc, char **argv)
{
	int fd, client_fd, port, pid;
	port = 45000;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		printf("Failed to init socket\n");
		return FALSE;
	}

	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);
	if (bind(fd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		printf("Unable to bind *:%d\n", port);
		return FALSE;
	}

	if (listen(fd, SOMAXCONN) == -1) {
		printf("Socket execute error\n");
		return FALSE;
	}

	printf("Server is listening on *:%d\n", port);
	while(TRUE) {
		
		client_fd = accept(fd, (struct sockaddr *) NULL, NULL);
		
		if ((pid = Fork()) == 0) {
			close(fd);
			request_handler(client_fd);
			close(client_fd);
			exit(0);
		}
	
		close(fd);
	}

	close(fd);
	return TRUE;
}

void request_handler(int sockfd)
{
	char *buf = "Hello\n";
	send(sockfd, buf, strlen(buf), 0);
	close(sockfd);
}
