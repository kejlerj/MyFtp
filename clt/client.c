/*
** EPITECH PROJECT, 2017
** client.c
** File description:
**
*/

#include "ftp.h"

void handle_error(char *str)
{
	fprintf(stderr, "Error : %s\n", str);
	fprintf(stderr, "%s\n", strerror(errno));
	exit(84);
}

int create()
{
	int sockfd;
	struct protoent *pe;

	if (!(pe = getprotobyname("TCP")))
		handle_error("Protocol");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
		handle_error("Socket creation");

	return (sockfd);
}

int rcv_mes(int sockfd, char **str)
{
	int size = 1;
	size_t buf_size = 1024;
	char *cmd = NULL;
	
	if (!(cmd = malloc(sizeof(char) * buf_size)))
		handle_error("Malloc");
	size = read(sockfd, cmd, buf_size);
	if (!(*str = realloc(*str, sizeof(char) * (size + 1))))
			handle_error("Realloc");
	*str = strncpy(*str, cmd, size);
	(*str)[size] = '\0';
	if (cmd)
		free(cmd);
	return (size);
}

void action(int sockfd)
{
	char *cmd = NULL;
	char *str = NULL;
	size_t buf_size = 32;
	int size = 0;
	
	while (1) {	
		size = getline(&cmd, &buf_size, stdin);
		if (size < 2)
			write(sockfd, "\0", 1);
		else
			write(sockfd, cmd, size - 1);
		size = rcv_mes(sockfd, &str);
		write(1, str, size);
		check(str, sockfd);
	}
	close(sockfd);
}

void connecting(int sockfd, char *ip, int port)
{
	struct sockaddr_in addr;
	int size = 0;
	char *str = NULL;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		close(sockfd);
		handle_error("Connect");
	}
	size = rcv_mes(sockfd, &str);
	write(1, str, size);
	if (str)
		free(str);
	action(sockfd);
}

int main(int argc, char **argv)
{
	int port;
	char *ip;
	int sockfd;

	if (argc == 3) {
		ip = argv[1];
		port = atoi(argv[2]);
		sockfd = create();
		connecting(sockfd, ip, port);
	}
	return (0);
}
