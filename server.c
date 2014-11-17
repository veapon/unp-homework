#include "server.h"

#define TRUE 1
#define FALSE 0
#define MAXLINE 4096

void request_handler(int);

int main(int argc, char **argv)
{
	int listenfd, connectfd, port;
	socklen_t client_len;
	struct sockaddr_in serv, cli;
	pid_t pid;

	port = 45000;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		printf("Failed to init socket\n");
		return FALSE;
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		printf("Unable to bind *:%d\n", port);
		return FALSE;
	}

	if (listen(listenfd, SOMAXCONN) == -1) {
		printf("Socket execute error\n");
		return FALSE;
	}

	printf("Server is listening on *:%d\n", port);
	while(TRUE) {
		client_len = sizeof(cli);
		connectfd = accept(listenfd, (struct sockaddr *) &cli, &client_len);

		/*
		while ((bytes_read = read(connectfd, buf, MAXLINE)) > 0) {
			printf("Message recieved: %s\n", buf);
			write(connectfd, buf, bytes_read);	
		}
		*/
		if ((pid = fork()) == 0) {
			close(listenfd);
			request_handler(connectfd);
			close(connectfd);
			exit(0);
		}

		close(connectfd);
	}

	close(listenfd);
	return TRUE;
}

void request_handler(int fd)
{
	int bytes_read;
	char buf[MAXLINE];

	while ((bytes_read = read(fd, buf, MAXLINE)) > 0) {
		printf("Client Message: %s, pid: %d, ppid: %d\n", buf, getpid(), getppid());
		write(fd, buf, bytes_read);	
	}

	close(fd);
}
