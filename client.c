#include <stdlib.h>
#include <stdio.h>

#include "client.h"

#define FALSE 0
#define TRUE 1
#define BUFSIZE 4096

int main(int argc, char **argv)
{
	/*
	char server[15];
	printf("Please input server address: ");
	if (fgets(server, BUFSIZE, stdin) == NULL) {
		printf("\nUnable to read from stdin.");
		return TRUE;
	}
	printf("%s", server);
	*/
	int fd;
	char *serv_ip = "127.0.0.1";
	struct sockaddr_in serv;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(45000);
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

	char *str, buf[BUFSIZE];
	str = (char *)calloc(BUFSIZE, sizeof(char));
	/*printf("argc: %d\n", argc);*/
	if (argc > 1) {
		str = argv[1];
	} else {
		strcpy(str, "client msg");
	}
	write(fd, str, strlen(str));
	
	read(fd, buf, BUFSIZE);
	printf("Server response: %s\n", buf);
	close(fd);
	return TRUE;
}
