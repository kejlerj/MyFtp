/*
** EPITECH PROJECT, 2017
** fct.c
** File description:
** functions
*/

#include "srv.h"

void fct_quit(user_t *user, char *str)
{
	write(user->csock, CODE_221, strlen(CODE_221));
	printf("Connection close.\n");
}

void fct_pasv(user_t *user, char *str)
{
	int i = 100;
	int fd = 0;

	while (i < 5000	&& (fd = create(i)) == -1)
		i++;
	write(user->csock, "227", strlen("227"));
}

void fct_retr(user_t *user, char *str)
{
	write(user->csock, "150", strlen("150"));
}

void fct_cwd(user_t *user, char *str)
{
	int size = strlen(&str[4]) - 2;
	char *file = &str[4];

	file[size] = '\0';
	if (chdir(file) == -1)
		write(user->csock, CODE_431, strlen(CODE_431));
	else
		write(user->csock, CODE_250, strlen(CODE_250));
}

void fct_cdup(user_t *user, char *str)
{
	if (chdir("..") == -1)
		write(user->csock, CODE_200, strlen(CODE_200));
	else
		write(user->csock, CODE_250, strlen(CODE_250));
}

void fct_pwd(user_t *user, char *str)
{
	char *buf = NULL;
	char *path = NULL;
	size_t buf_size = 1024;
	int size = 0;

	if (!(buf = malloc(sizeof(char) * buf_size)))
		handle_error("Malloc");
	getcwd(buf, buf_size);
	size = strlen(buf) + strlen(CODE_257A) + strlen(CODE_257B);
	if (!(path = malloc(sizeof(char) * size)))
		handle_error("Malloc");
	path = strcpy(path, CODE_257A);
	path = strcat(path, buf);
	path = strcat(path, CODE_257B);
	write(user->csock, path, strlen(path));
	if (buf)
		free(buf);
	if (path)
		free(path);
}

void fct_list(user_t *user, char *str)
{
	DIR *d;
	struct dirent *dir;

	if (!(d = opendir(".")))
		write(user->csock, CODE_431, strlen(CODE_431));
	else {
		write(user->csock, CODE_150, strlen(CODE_150));
		while ((dir = readdir(d))) 
			printf("%s\n", dir->d_name);
		closedir(d);
		write(user->csock, CODE_226, strlen(CODE_226));
	}
}

void fct_user(user_t *user, char *str)
{
	if (strncmp(&str[5], USERNAME, strlen(&str[5]) -2) == 0)
		user->user_auth = 1;
	if (user->user_auth == 1) {
		if (user->pass_auth == 0)
			write(user->csock, CODE_331, strlen(CODE_331));
		else {
			write(user->csock, CODE_230, strlen(CODE_230));
			user->connected = 1;
		}
	}
	else
		write(user->csock, CODE_331, strlen(CODE_331));
}

void fct_pass(user_t *user, char *str)
{
	if (strncmp(&str[5], PASSWORD, strlen(&str[5]) -2) == 0)
		user->pass_auth = 1;
	if (user->pass_auth == 1) {
		if (user->user_auth == 0) {
			user->pass_auth = 0;
			write(user->csock, CODE_530, strlen(CODE_530));
		}
		else {
			write(user->csock, CODE_230, strlen(CODE_230));
			user->connected = 1;
		}
	}
	else {
		user->pass_auth = 0;
		write(user->csock, CODE_530, strlen(CODE_530));
	}
}

void fct_help(user_t *user, char *str)
{
	write(user->csock, CODE_214, strlen(CODE_214));
}

int set_options(char *str)
{
	char *fct[] = {"PASV", "RETR", "CWD", "CDUP", "PWD", "LIST", "USER", "PASS",
		"HELP", "QUIT", 0};
	for (int i = 0; i < NB_FCT; i++) {
		if (strncmp(fct[i], str, strlen(fct[i])) == 0)
			return (i);
	}
	return (-1);
}

static void tab(void (*tab_ft[NB_FCT])(user_t *user, char *str))
{
	tab_ft[0] = &fct_pasv;
	tab_ft[1] = &fct_retr;
	tab_ft[2] = &fct_cwd;
	tab_ft[3] = &fct_cdup;
	tab_ft[4] = &fct_pwd;
	tab_ft[5] = &fct_list;
	tab_ft[6] = &fct_user;
	tab_ft[7] = &fct_pass;
	tab_ft[8] = &fct_help;
	tab_ft[9] = &fct_quit;
}

void check(char *str, user_t *user)
{
	void (*tab_ft[NB_FCT])();
	int opt = 0;

	tab(tab_ft);
	if ((opt = set_options(str)) == -1)
		write(user->csock, CODE_500, strlen(CODE_500));
	else if (user->connected == 0 && (opt != 6 && opt != 7 && opt != 9))
		write(user->csock, CODE_530, strlen(CODE_530));
	else
		tab_ft[opt](user, str);
}
