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

int main(int argc, char const *argv[])
{
	// Servidor
	// Processa argumentos da linha de comando
	// porta_servidor tam_buffer
	Args *argumentos = get_args(argc, argv);
	
	// Faz abertura passiva e aguarda conexão
	int server = create_socket();

	struct sockaddr_in server_config = configure_server(argumentos);

	bind_socket(server, server_config);

	listen_socket(server);

	// Recebe a string com o nome do arquivo
	int client = accept_socket(server, server_config);

	read(client , argumentos->file, sizeof(argumentos->file));

	// Chama gettimeofday para tempo inicial
	struct timeval start, end;
  	gettimeofday(&start, NULL);

	unsigned int bytes = 0;

	// Abre arquivo que vai ser lido
	FILE *fp = open_file(argumentos->file, "r");

	// Loop lê o arquivo, um buffer por vez até fread retornar zero
    while(fread(argumentos->buffer, sizeof(char), argumentos->len_buffer, fp) > 0)
    {
		// Envia o buffer lido
		send(client, argumentos->buffer, strlen(argumentos->buffer) , 0);
		// Contabiliza bytes enviados
		bytes += strlen(argumentos->buffer);
		memset(argumentos->buffer, 0x0, argumentos->len_buffer);
    }
	// Fim_loop
	
	// Fecha conexão e arquivo
	close(client);
	fclose(fp);
	
	// Chama gettimeofday para tempo final e calcula tempo gasto
	gettimeofday(&end, NULL);
	float tempo_sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1E-6;
	
	// Imprime nome arquivo e número de bytes enviados
	printf("Enviado = %s (%u bytes em %3.6f s)\n", argumentos->file, bytes, tempo_sec);
	
	// Fim_servidor.
	close(server);
	free_args(argumentos);
	return 0;
}
 