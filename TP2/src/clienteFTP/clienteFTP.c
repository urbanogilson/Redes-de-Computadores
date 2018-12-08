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
#include "tp_socket.h"		// Funções auxiliares para uso de sockets UDP

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
    parm->len_frame = parm->len_buffer + 15;
    parm->frame = (char*) malloc(parm->len_frame * sizeof(char));
    parm->ack = (char*) malloc(sizeof(char));
    strcpy(parm->ack, "0");
    parm->nack = (char*) malloc(sizeof(char));
    strcpy(parm->nack, "1");
    memset(parm->buffer, 0, parm->len_buffer);
    printf("HOST: %s\nPORT: %d\nFILE: %s\nBUFFER: %d\n", parm->host, parm->port, parm->file, parm->len_buffer);
    return parm;
}

void free_args(Args *parm)
{
    free(parm->host);
    free(parm->file);
    free(parm->buffer);
    free(parm->frame);
    free(parm->ack);
    free(parm->nack);
	free(parm);
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