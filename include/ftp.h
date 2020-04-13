/*
** EPITECH PROJECT, 2017
** ftp.h
** File description:
** header
*/

#ifndef FTP_H_
#define FTP_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define	NB_FCT	3

int rcv_mes(int, char **);
void handle_error(char *);
void check(char *, int);

#endif
