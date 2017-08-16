/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     the main server component                                             */
/*                                                                           */
/*****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef IPS_SERVER

#define IPS_SERVER

typedef struct{
	int sock_fd;
	struct sockaddr_in serv_addr;
	void * (*thread_handler)(void *);
}server_t;

typedef struct{
	int client_fd;
	int client_id;
	struct sockaddr_in addr;
	int num_work_queued;
}data_t;

/**
 * Most probably would like to use like this
 * init_server(AF_INET, SOCK_STREAM, 0, AF_INET, INADDR_ANY, portno)
 * return a server_t which you can do
 * register_handler(server_t server, void (*thread_handler)(int sockfd))
 * start_server(server_t server)
 */
server_t init_server(int domain, int type, int protocol,
	sa_family_t sin_family, uint32_t, int portno);

/**
 * The thread_handler is a function that should handle a connection
 * 
 * the int in the thread_handler should be a file descriptor of a client
 * void (*thread_handler)(int)
 */
void register_handler(server_t *server, void* (*thread_handler)(void*));

/**
 * Start executing the server
 */
void server_listen(server_t *server);

#endif
