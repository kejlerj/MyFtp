/*
** EPITECH PROJECT, 2017
** server.c
** File description:
** main file
*/

#include "srv.h"

void handle_error(char *str)
{
	fprintf(stderr, "Error : %s\n", str);
	fprintf(stderr, "%s\n", strerror(errno));
	exit(84);
}

int create(int port)
{
	int sockfd;
	struct sockaddr_in addr;
	struct protoent *pe;

	if (!(pe = getprotobyname("TCP")))
		handle_error("Protocol");
	if (!(sockfd = socket(AF_INET, SOCK_STREAM, pe->p_proto)))
		handle_error("Socket creation");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		return (-1);
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

void action(int csock, struct sockaddr_in caddr)
{
	int pid = 0;
	char *str = NULL;
	int size = 0;
	user_t *user = NULL;

	if ((pid = fork()) != 0) {
		printf("Connection from %s\n", inet_ntoa(caddr.sin_addr));
		if (!(user = malloc(sizeof(user_t))))
			handle_error("Malloc");
		user->user_auth = 0;
		user->pass_auth = 0;
		user->connected = 0;
		user->csock = csock;
		do {
			size = rcv_mes(user->csock, &str);
			check(str, user);
		} while (strcmp(str, "QUIT") != 0);
		close(user->csock);
	}
}

void listening(int sockfd)
{
	int csock;
	struct sockaddr_in caddr;
	int s_size = sizeof(caddr);

	if ((listen(sockfd, 5)) == -1) {
		handle_error("Listen");
		close(sockfd);
	}
	while (1) {
		if ((csock = accept(sockfd, (struct sockaddr *)&caddr, &s_size))!=-1) {
			write(csock, CODE_220, strlen(CODE_220));
			action(csock, caddr);
		}
	}
	close(sockfd);
}

void usage()
{
	printf("USAGE: ./server port path\n");
	printf("\tport is the port number on which the server socket listens\n");
	printf("\tpath is the path to the home directory for the Anonymous user\n");
}

int main(int argc, char **argv)
{
	int port = 0;
	char *path;
	int sockfd;

	if (argc == 1 || strcmp(argv[1], "-help") == 0)
		usage();
	else if (argc == 3) {
		port = atoi(argv[1]);
		path = argv[2];
		if ((sockfd = create(port)) == -1)
			return (-1);
		listening(sockfd);
	}
	return (0);
}
