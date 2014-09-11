#include "server.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char **argv)
{
	int fd, client_fd;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		printf("Failed to init socket\n");
		return FALSE;
	}

	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_port = htons(56789);
	if (bind(fd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		printf("Unable to bind *:56789\n");
		return FALSE;
	}

	if (listen(fd, SOMAXCONN) == -1) {
		printf("Socket execute error\n");
		return FALSE;
	}

	printf("Server is listening on *:56789\n");
	while(TRUE) {
		client_fd = accept(fd, (struct sockaddr *) NULL, NULL);
		if (client_fd == -1) {
			break;
		}
	}
	return TRUE;
}
