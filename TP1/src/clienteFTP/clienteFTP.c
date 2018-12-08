#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 	// Internet Protocol family
#include <sys/types.h> 		// Data types
#include <netinet/in.h> 	// Internet address family
#include <sys/time.h> 		// Time types
#include <unistd.h>         // Standard symbolic constants and types
#include <arpa/inet.h>      // Definitions for internet operations
#include "clienteFTP.h"

Args *get_args(int argc, char const *argv[])
{
    if (argc < 5)
    {
        printf("ERRO: Ausencia de parametros (HOST, PORT, FILE, BUFFER)\n");
        exit(1);
    }
    Args *parm = (Args*) malloc(sizeof(Args));
    parm->host = (char*) malloc((strlen(argv[1]+1)) * sizeof(char));
    strcpy(parm->host, argv[1]);
    parm->port = atoi(argv[2]);
    parm->file = (char*) malloc((strlen(argv[3]+1)) * sizeof(char));
    strcpy(parm->file, argv[3]);
    parm->len_buffer = atoi(argv[4]);
    parm->buffer = (char*) malloc(parm->len_buffer * sizeof(char));
    memset(parm->buffer, 0, parm->len_buffer);
    // printf("HOST: %s\nPORT: %d\nFILE: %s\nBUFFER: %d\n", parm->host, parm->port, parm->file, parm->len_buffer);
    return parm;
}

void free_args(Args *parm)
{
    free(parm->file);
    free(parm->buffer);
	free(parm);
}

int create_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("ERRO: Falha na criacao do socket\n");
		exit(1);
	}
	return sock;
}

struct sockaddr_in configure_server(Args *parm)
{
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(parm->port);

    if (strlen(parm->host) < strlen("0.0.0.0")) // loopback "127.0.0.1" 
    {
        server.sin_addr.s_addr = INADDR_ANY;
        // printf("WARNING: IP automatico\n");
    }
    else if (inet_pton(AF_INET, parm->host, &server.sin_addr) <= 0) 
    {
        printf("ERRO: Endereco nao suportado\n");
	    exit(1);
    }
    return server;
}

int connect_server(int sock, struct sockaddr_in server)
{
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("ERRO: Falha ao conectar ao servidor\n");
        exit(1);
    }
    // printf("Conectado\n");
    return 0;
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