#include "main.h"

#define FALSE 0
#define TRUE 1
#define BUFSIZE 4096

int main(int argc, char **argv)
{
	int fd;
	char *serv_ip = "127.0.0.1";
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);
	if (1 != inet_pton(AF_INET, serv_ip, &serv.sin_addr)) {
		printf("Unable to initialize socket\n");
		return FALSE;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		printf("Unable to initialize socket\n");
		return FALSE;
	} 

	if (connect(fd, (struct sockaddr *) &serv, sizeof(serv)) < 0) {
		printf("Unable to connect server: %s", serv_ip);
		return FALSE;
	}

	char buf[BUFSIZE];
	while (read(fd, buf, BUFSIZE) > 0) {
		printf("%s", buf);
	}
	return TRUE;
}
