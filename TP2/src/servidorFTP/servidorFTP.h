#ifndef servidorFTP
#define servidorFTP

typedef struct Args
{
    int port, len_buffer, len_frame, len_file;
    char *frame, *buffer, *file, *ack, *nack;
}Args;

Args *get_args(int argc, char const *argv[]);

void free_args(Args *parm);
int create_socket();

struct sockaddr_in configure_server(Args *parm);
int bind_socket(int sock, struct sockaddr_in server);
int listen_socket(int sock);
int accept_socket(int sock, struct sockaddr_in server);
FILE *open_file(char *path, char *mode);

#endif // servidorFTP