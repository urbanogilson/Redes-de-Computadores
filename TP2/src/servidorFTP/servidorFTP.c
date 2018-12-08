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
    parm->len_file = 50;
    parm->file = (char*) malloc(parm->len_file * sizeof(char));
    parm->port = atoi(argv[1]);
    parm->len_buffer = atoi(argv[2]);
    parm->buffer = (char*) malloc(parm->len_buffer * sizeof(char));
    parm->len_frame = parm->len_buffer + 15;
    parm->frame = (char*) malloc(parm->len_frame * sizeof(char));
    parm->ack = (char*) malloc(sizeof(char));
    sprintf(parm->ack, "0");
    parm->nack = (char*) malloc(sizeof(char));
    sprintf(parm->nack, "0");
    memset(parm->buffer, 0, parm->len_buffer);
    printf("PORT: %d\nBUFFER: %d\n", parm->port, parm->len_buffer);
    return parm;
}

void free_args(Args *parm)
{
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