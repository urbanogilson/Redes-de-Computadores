all:	servidorFTP

servidorFTP:	main.o servidorFTP.o tp_socket.o
	gcc main.o servidorFTP.o tp_socket.o -o servidorFTP

main.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g main.c

servidorFTP.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g servidorFTP.c

tp_socket.o:
	gcc -c tp_socket.c

clean:
	rm -rf *o servidorFTP
