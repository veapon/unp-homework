#include "client.h"

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
	serv.sin_port = htons(13);
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
		printf("Unable to connect server: %s\n", serv_ip);
		return FALSE;
	}

	int n;
	char buf[BUFSIZE + 1];
	while ((n = read(fd, buf, BUFSIZE)) > 0) {
		buf[n] = 0;	
		printf("%s\n", buf);
	}
	if (n < 0) {
		printf("Read error\n");
	}
	close(fd);
	return TRUE;
}
