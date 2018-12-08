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

int main(int argc, char const *argv[])
{
	// Cliente:
	// Processa argumentos da linha de comando:
	// host_do_servidor porta_servidor nome_arquivo tam_buffer
	Args *argumentos = get_args(argc, argv);
	
	// Chama gettimeofday para tempo inicial
	struct timeval start, end;
  	gettimeofday(&start, NULL);

	unsigned int bytes = 0;

	// Faz abertura ativa a host_do_servidor
	int client = create_socket();

	struct sockaddr_in client_config = configure_server(argumentos);

	connect_server(client, client_config);

	// Envia string com nome do arquivo
	send(client, argumentos->file, strlen(argumentos->file), 0);

	// Abre arquivo que vai ser gravado
	FILE *fp = open_file(argumentos->file, "w+");

	// Loop recv buffer até que perceba que o arquivo acabou
	do
	{
		memset(argumentos->buffer, 0x0, argumentos->len_buffer);
		recv(client, argumentos->buffer, argumentos->len_buffer, 0);
		// Escreve bytes do buffer no arquivo
		fwrite(argumentos->buffer, sizeof(char), strlen(argumentos->buffer), fp);
		// Atualiza contagem de bytes recebidos
		bytes += strlen(argumentos->buffer);
	} while((int) strlen(argumentos->buffer) == argumentos->len_buffer);
	// Fim_loop
	
	// Fecha conexão e arquivo
	close(client);
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
 