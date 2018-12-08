all:	servidorFTP

servidorFTP:	main.o servidorFTP.o
	gcc main.o servidorFTP.o -o servidorFTP

main.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g main.c

servidorFTP.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g servidorFTP.c

clean:
	rm -rf *o servidorFTP
