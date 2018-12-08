#include <stdio.h>
#include <stdlib.h>
#include <string.h>				// String
#include <sys/socket.h> 	// Internet Protocol family
#include <sys/types.h> 		// Data types
#include <netinet/in.h> 	// Internet address family
#include <sys/time.h> 		// Time types
#include <unistd.h>         // Standard symbolic constants and types
#include <arpa/inet.h>      // Definitions for internet operations
#include "clienteFTP.h"
#include "tp_socket.h"		// Funções auxiliares para uso de sockets UDP

int main(int argc, char const *argv[])
{
	// Cliente
	tp_init();

	// Processa argumentos da linha de comando:
	// host_do_servidor porta_servidor nome_arquivo tam_buffer
	Args *argumentos = get_args(argc, argv);
	int id = 0;
	char *buffer;

	// Chama gettimeofday para tempo inicial
	struct timeval start, end;
  	gettimeofday(&start, NULL);

	unsigned int bytes = 0;

	// Socket
	int sockClient = tp_socket(0);

	so_addr server;
    memset(&server, 0, sizeof(server)); 

	tp_build_addr(&server, NULL, argumentos->port);

	// Envia string com nome do arquivo
	tp_sendto(sockClient, argumentos->file, strlen(argumentos->file), &server);
  
	// Abre arquivo que vai ser gravado
	FILE *fp = open_file(argumentos->file, "w+");

	// Loop recv buffer até que perceba que o arquivo acabou
	while(1)
	{
		memset(argumentos->frame, 0x0, argumentos->len_frame);
		if(tp_recvfrom(sockClient, argumentos->frame, argumentos->len_frame, &server) > 0)
		{
			if(strcmp("0", argumentos->frame) == 0)
			{
				break;
			}
			else
			{
				char *ID = strtok(argumentos->frame, "|");			
				int id_rec = atoi(ID);
				if(id_rec == id + 1)
				{
					buffer = strtok(NULL, "\0");
					bytes += strlen(buffer);
					fwrite(buffer, sizeof(char), strlen(buffer), fp);
					id++;
					if(tp_sendto(sockClient, argumentos->ack, strlen(argumentos->ack), &server) < 0)
					{
						printf("ERRO: Envio do ACK\n");
						return 1;
					}
				}
				else
				{
					if(tp_sendto(sockClient, argumentos->nack, strlen(argumentos->nack), &server) < 0)
					{
						printf("ERRO: Envio do NACK\n");
						return 1;
					}
				}
			}
		}
		else
		{
			if(tp_sendto(sockClient, argumentos->nack, strlen(argumentos->nack), &server) < 0)
			{
				printf("ERRO: Envio do NACK\n");
				return 1;
			}
		}
	}
	// Fim_loop
	
	// Fecha conexão e arquivo
	close(sockClient);
	fclose(fp);
	
	// Chama gettimeofday para tempo final e calcula tempo gasto
	gettimeofday(&end, NULL);
	float tempo_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
	float kbps = bytes*8E-3/tempo_sec;

	// Imprime resultado
	printf("Buffer = %5u byte(s), %10.2f kbps (%u bytes em %3.6f s)\n", bytes, kbps, bytes, tempo_sec);

	// Fim cliente.
	free_args(argumentos);
	return 0;
}
 