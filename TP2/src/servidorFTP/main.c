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
#include "tp_socket.h"		// Funções auxiliares para uso de sockets UDP

int main(int argc, char const *argv[])
{
	// Servidor
	tp_init();

	// Processa argumentos da linha de comando
	// porta_servidor tam_buffer
	Args *argumentos = get_args(argc, argv);
	int id = 1;

	// Chama gettimeofday para tempo inicial
	struct timeval start, end;
  	gettimeofday(&start, NULL);

	unsigned int bytes = 0;

	// Faz abertura passiva e aguarda conexão
	int sockServer =  tp_socket(argumentos->port);

	// // Recebe a string com o nome do arquivo
	so_addr client;
	tp_recvfrom(sockServer, argumentos->file, argumentos->len_file, &client);
	
	// Abre arquivo que vai ser lido
	FILE *fp = open_file(argumentos->file, "r");

	// Loop lê o arquivo, um buffer por vez até fread retornar zero
	while(!feof(fp))		
    {
		if(strcmp("0", argumentos->ack) == 0)
		{
			memset(argumentos->buffer, 0x0, argumentos->len_buffer);
			memset(argumentos->frame, 0x0, argumentos->len_frame);	
			char ID[argumentos->len_frame - argumentos->len_buffer];
			sprintf(ID, "%d", id);
            strcat(argumentos->frame, ID);
            strcat(argumentos->frame, "|");
			fread(argumentos->buffer, sizeof(char), argumentos->len_buffer, fp);
			strcat(argumentos->frame, argumentos->buffer);
			// Envia o frame com último buffer lido
			if (tp_sendto(sockServer, argumentos->frame, argumentos->len_frame, &client) < 0)
			{
				printf("ERRO: Frame nao pode ser enviado\n");
				return 1;
			}
			id++;
		}
		else
		{
			if (tp_sendto(sockServer, argumentos->frame, argumentos->len_frame, &client) < 0)
			{
				printf("ERRO: Frame nao pode ser enviado\n");
				return 1;
			}
		}
		// Recebe ACK | NACK
		if(tp_recvfrom(sockServer, argumentos->ack, sizeof(argumentos->ack), &client) < 0)
		{
			printf("ERRO: Recebimento do ACK | NACK\n");
			return 1;
		}	
		// Contabiliza bytes enviados
		bytes += strlen(argumentos->buffer);
	}
	// Fim_loop

    // Envia caracter que fecha a conexão
    if(tp_sendto(sockServer, "0", 1, &client) < 0)
	{
        printf("ERRO: Flag de fechamento\n");
        return 1;
    }
	
	// Fecha conexão e arquivo
	fclose(fp);
	
	// Chama gettimeofday para tempo final e calcula tempo gasto
	gettimeofday(&end, NULL);
	float tempo_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
	
	// Imprime nome arquivo e número de bytes enviados
	printf("Enviado = %s (%u bytes em %3.6f s)\n", argumentos->file, bytes, tempo_sec);
	
	// Fim_servidor.
	free_args(argumentos);
	return 0;
}