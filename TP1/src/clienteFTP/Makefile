all:	clienteFTP

clienteFTP:		main.o clienteFTP.o
	gcc main.o clienteFTP.o -o clienteFTP

main.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g main.c

clienteFTP.o:
	gcc -c -Wall -Wextra -Werror -std=c99 -pedantic -g clienteFTP.c

clean:
	rm -rf *o clienteFTP
