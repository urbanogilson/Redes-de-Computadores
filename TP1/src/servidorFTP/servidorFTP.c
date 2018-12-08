#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 	// Internet Protocol family
#include <sys/types.h> 		// Data types
#include <netinet/in.h> 	// Internet address family
#include <sys/time.h> 		// Time types
#include <unistd.h>         // Standard symbolic constants and types
#include <arpa/inet.h>      // Definitions for internet operations
#include "servidorFTP.h"

Args *get_args(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("ERRO: Ausencia de parametros (PORT, BUFFER)\n");
        exit(EXIT_FAILURE);
    }
    Args *parm = (Args*) malloc(sizeof(Args));
    parm->file = (char*) malloc(50 * sizeof(char));
    parm->port = atoi(argv[1]);
    parm->len_buffer = atoi(argv[2]);
    parm->buffer = (char*) malloc(parm->len_buffer * sizeof(char));
    memset(parm->buffer, 0, parm->len_buffer);
    // printf("PORT: %d\nBUFFER: %d\n", parm->port, parm->len_buffer);
    return parm;
}

void free_args(Args *parm)
{
    free(parm->buffer);
	free(parm);
}

int create_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("ERRO: Falha na criacao do socket\n");
		exit(EXIT_FAILURE);
	}
	return sock;
}

struct sockaddr_in configure_server(Args *parm)
{
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    // server.sin_addr.s_addr = inet_addr("74.125.235.20");
    server.sin_family = AF_INET;
    server.sin_port = htons(parm->port);
    server.sin_addr.s_addr = INADDR_ANY;
    return server;
}

int bind_socket(int sock, struct sockaddr_in server)
{
    int bin_sock = bind(sock, (struct sockaddr *) &server, sizeof(server));
    if (bin_sock < 0)
    {
        printf("ERRO: Falha no bind\n");
        exit(EXIT_FAILURE);
    }
    return bin_sock;
}

int listen_socket(int sock)
{   
    int lis_sock = listen(sock, 3);
    if (lis_sock < 0)
    {
        printf("ERRO: Falha no listen\n");
        exit(EXIT_FAILURE);
    }
    return lis_sock;
}

int accept_socket(int sock, struct sockaddr_in server)
{
    int server_len = sizeof(server);
    int acp_sock = accept(sock, (struct sockaddr *)&server, (socklen_t *) &server_len);
    if (acp_sock < 0)
    {
		printf("ERRO: Falha na coneccao\n");
		exit(EXIT_FAILURE);
    }
    return acp_sock;
}

FILE *open_file(char *path, char *mode)
{
    FILE *fp;
	fp = fopen (path, mode);
	if (fp == NULL)
	{
		printf("ERRO: Falha na abertura do arquivo %s\n", path);
		exit(EXIT_FAILURE);
	}
    return fp;
}