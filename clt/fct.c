/*
** EPITECH PROJECT, 2017
** fct.c
** File description:
** functions
*/

#include "ftp.h"

void fct_221(int sockfd)
{
	close(sockfd);
	exit(0);
}

void fct_pasv(int sockfd)
{
	printf("in pasv\n");
}

void fct_retr(int sockfd)
{
	printf("in retr\n");
}

int set_options(char *str)
{
	char *fct[] = {"227", "RETR", "221", 0};
	for (int i = 0; i < NB_FCT; i++) {
		if (strncmp(fct[i], str, 3) == 0)
			return (i);
	}
	return (-1);
}

static void tab(void (*tab_ft[NB_FCT])(int csock))
{
	tab_ft[0] = &fct_pasv;
	tab_ft[1] = &fct_retr;
	tab_ft[2] = &fct_221;
}

void check(char *str, int sockfd)
{
	void (*tab_ft[NB_FCT])();
	int opt = 0;

	tab(tab_ft);
	if ((opt = set_options(str)) != -1)
		tab_ft[opt](sockfd);
}
