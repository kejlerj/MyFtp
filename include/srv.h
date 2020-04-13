/*
** EPITECH PROJECT, 2017
** srv.h
** File description:
** header
*/

#ifndef SRV_H_
#define SRV_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define	NB_FCT	10

#define	USERNAME	"Anonymous"
#define PASSWORD	""

#define CODE_150	"150 File status okay; about to open data connection.\r\n"
#define CODE_200	"200 Command okay.\r\n"
#define CODE_214	"214 Help message.\nOn how to use the server or the meaning of a particular non-standard command. This reply is useful only to the human user.\r\n"
#define CODE_220	"220 Service ready for new user.\r\n"
#define CODE_221	"221 Service closing control connection.\nLogged out if appropriate.\r\n"
#define CODE_226	"226 Closing data connection.\r\n"
#define CODE_230	"230 User logged in, proceed.\r\n"
#define CODE_250	"250 Requested file action okay, completed.\r\n"
#define CODE_257A	"257 \"<"
#define CODE_257B	">\"\r\n"
#define CODE_331	"331 User name okay, need password.\r\n"
#define CODE_332	"332 Need account for login.\r\n"
#define CODE_431	"431 No such directory\r\n"
#define CODE_500	"500 Syntax error, command unrecognized.\r\n"
#define CODE_530	"530 Not logged in.\r\n"

typedef struct user_t {
	int user_auth;
	int pass_auth;
	int connected;
	int csock;
} user_t;

void handle_error(char *str);
void check(char *, user_t *);
int create(int);

#endif
