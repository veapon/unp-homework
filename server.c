#include "server.h"

#define TRUE 1
#define FALSE 0
#define MAXLINE 4096

void request_handler(int);

int main(int argc, char **argv)
{
	int listenfd, connectfd, port, i, maxi, maxfd, sockfd;
	socklen_t client_len;
	struct sockaddr_in serv, cli;
	//pid_t pid;
	

	// select version
	int nready, client[FD_SETSIZE];
	fd_set rset, allset;

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
			connectfd = accept(listenfd, (struct sockaddr *) &cli, &client_len);

			for (i = 0; i < FD_SETSIZE; i++) {
				if (client[i] < 0) {
					client[i] = connectfd;
					break;
				}
			}
			
			if (i == FD_SETSIZE) {
				printf("too many clients\n");
				exit(0);
			}

			if (connectfd > maxfd) {
				maxfd = connectfd;
			}

			if (i > maxi) {
				maxi = i;
			}
			printf("i: %d, maxi: %d\n", i, maxi);

		}

		for(i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0) {
				continue;
			}
			
			if (FD_ISSET(sockfd, &rset)) {
				request_handler(sockfd);
				FD_CLR(sockfd, &allset);
				client[i] = -1;
			}
		}

		/*
		 * I/O blocking version
		 */
		/*
		while ((bytes_read = read(connectfd, buf, MAXLINE)) > 0) {
			printf("Message recieved: %s\n", buf);
			write(connectfd, buf, bytes_read);	
		}

		*/

		/* 
		 * fork version
		 */
		/*
		if ((pid = fork()) == 0) {
			close(listenfd);
			request_handler(connectfd);
			close(connectfd);
			exit(0);
		}
		*/



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
		printf("Client Message: %s\n", buf);
		write(fd, buf, bytes_read);	
	}

	close(fd);
}
