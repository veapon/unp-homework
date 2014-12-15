#include "server.h"

#define TRUE 1
#define FALSE 0
#define BUFSIZE 4096

void request_handler(int);

/**
 * I/O blocking version
 */
/*
int main(int argc, char **argv)
{
	int listenfd, connfd, port, bytes_read;
	char buf[BUFSIZE];
	struct sockaddr_in serv;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		msg_error("Failed to init socket\n");
	}
	
	memset(&serv, 0, sizeof(serv));
	port = 45000;
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		msg_error("Bind error\n");
	}

	if (listen(listenfd, SOMAXCONN) == -1) {
		msg_error("Socket execute error\n");
	}
	
	printf("Server is listening on: %d\n", port);
	while(TRUE) {
		connfd = accept(listenfd, NULL, NULL);
		request_handler(connfd);

		close(connfd);
	}

	close(listenfd);
	return TRUE;
	
}
*/


/**
 * fork version
 */
int main(int argc, char **argv)
{
	int listenfd, connfd, port, bytes_read;
	char buf[BUFSIZE];
	pid_t pid;
	struct sockaddr_in serv;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		msg_error("Failed to init socket\n");
	}
	
	memset(&serv, 0, sizeof(serv));
	port = 45000;
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		msg_error("Bind error\n");
	}

	if (listen(listenfd, SOMAXCONN) == -1) {
		msg_error("Socket execute error\n");
	}
	
	printf("Server is listening on: %d\n", port);
	while(TRUE) {
		connfd = accept(listenfd, NULL, NULL);

		if ((pid = fork()) == 0) {
			request_handler(connfd);
			close(listenfd);
			close(connfd);
			exit(TRUE);
		}

		close(connfd);
	}

	close(listenfd);
	return TRUE;	
}

/*
 * select version
 */
/*
int main(int argc, char **argv)
{
	int listenfd, connfd, port, i, maxi, maxfd, sockfd;
	socklen_t client_len;
	struct sockaddr_in serv, cli;

	// select version
	int nready, client[FD_SETSIZE];
	fd_set rset, allset;

	port = 45000;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		msg_error("Failed to init socket\n");
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		msg_error("Bind error.\n");
	}

	// select version
	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++) {
		client[i] = -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
	printf("Server is listening on *:%d\n", port);
	while(TRUE) {
		// select version
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(listenfd, &rset)) {

			client_len = sizeof(cli);
			connfd = accept(listenfd, (struct sockaddr *) &cli, &client_len);

			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = connfd;
					break;
				}
			}
			
			if (i == FD_SETSIZE) {
				msg_error("Too many clients\n");
			}

			if (connfd > maxfd) {
				maxfd = connfd;
			}

			if (i > maxi) {
				maxi = i;
			}
		}

		for(i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0) {
				continue;
			}
			
			int bytes_read;
			char buf[BUFSIZE];
			while ((bytes_read = read(sockfd, buf, BUFSIZE)) > 0) {
				write(sockfd, buf, bytes_read);	
			}
			close(sockfd);
			FD_CLR(sockfd, &allset);
			client[i] = -1;
		}
	}

	close(listenfd);
	return TRUE;
}
*/
void request_handler(int fd)
{
	int bytes_read;
	char buf[BUFSIZE];

	while ((bytes_read = read(fd, buf, BUFSIZE)) > 0) {
		printf("Client Message: %s\n", buf);
		write(fd, buf, bytes_read);	
	}

	close(fd);
}
