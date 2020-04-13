##
## EPITECH PROJECT, 2017
## Makefile
## File description:
## Makefile for test
##

SRC_SRV	=	srv/server.c	\
		srv/fct.c

SRC_CLT	=	clt/client.c	\
		clt/fct.c

lFLAGS	=	-Wall -Wextra -Werror

SRV	=	server

CLT	=	myftp

all:	$(SRV) $(CLT)

$(SRV):
	gcc -o $(SRV) $(SRC_SRV) $(FLAGS) -Iinclude -g3

$(CLT):
	gcc -o $(CLT) $(SRC_CLT) $(FLAGS) -Iinclude -g3

clean:
	rm -f $(SRV)
	rm -f $(CLT)
	rm -f vgcore*

fclean:	clean

re:	fclean all

.PHONY:
	all clean fclean re
