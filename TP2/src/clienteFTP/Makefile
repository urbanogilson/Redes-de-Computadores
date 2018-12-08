all:	clienteFTP

clienteFTP:		main.o clienteFTP.o tp_socket.o
	gcc main.o clienteFTP.o tp_socket.o -o clienteFTP

main.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g main.c

clienteFTP.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g clienteFTP.c

tp_socket.o:
	gcc -c tp_socket.c

clean:
	rm -rf *o clienteFTP
