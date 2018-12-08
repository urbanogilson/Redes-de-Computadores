#ifndef clienteFTP
#define clienteFTP

typedef struct Args
{
    int port, len_buffer;
	char *file, *buffer, *host;
}Args;

Args *get_args(int argc, char const *argv[]);
void free_args(Args *parm);
int create_socket();

struct sockaddr_in configure_server(Args *parm);
int connect_server(int sock, struct sockaddr_in server);
FILE *open_file(char *path, char *mode);

#endif // clienteFTP