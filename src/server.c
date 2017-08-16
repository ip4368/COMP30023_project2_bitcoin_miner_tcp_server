/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "constants.h"
#include "server.h"
#include "signal_handler.h"
#include "alloc.h"
#include "logger.h"
#include "work_daemon.h"

/*****************************************************************************/
/*
 * Helper prototype
 */

static void socker_exception();
static void port_exception();
static void acceptance_exception();

/*****************************************************************************/

server_t init_server(int domain, int type, int protocol,
        sa_family_t sin_family, uint32_t s_addr, int portno){
    server_t server;

    server.sock_fd = socket(domain, type, protocol);
    if(server.sock_fd < 0){
        socker_exception();
    }

    bzero((char *) &server.serv_addr, sizeof(server.serv_addr));
    server.serv_addr.sin_family = sin_family;
    server.serv_addr.sin_addr.s_addr = s_addr;
    server.serv_addr.sin_port = htons(portno);

    if(bind(server.sock_fd, (struct sockaddr *) &server.serv_addr,
            sizeof(server.serv_addr)) < 0){
        port_exception(portno);
    }

    listen(server.sock_fd, MAX_CONNECTION);

    return server;
}

void register_handler(server_t *server, void* (*thread_handler)(void*)){
    server->thread_handler = thread_handler;
}

void server_listen(server_t *server){
    struct sockaddr_in client_addr;
    uint client_len = sizeof(client_addr);

    work_daemon_t work_daemon;
    set_daemon(&work_daemon);
    /*
     * this also start a thread
     */
    init_work_daemon();

    pthread_t *client_threads = 
        safe_malloc(sizeof(*client_threads) * MAX_CONNECTION);

    data_t *clients = safe_malloc(sizeof(*clients) * MAX_CONNECTION);
    bzero((char *) clients, sizeof(*clients) * MAX_CONNECTION);

    uint curr_client_id = 0;

    while(1){
        clients[curr_client_id].client_fd = accept(server->sock_fd,
            (struct sockaddr *) &client_addr,
            &client_len);
        clients[curr_client_id].addr = client_addr;

        clients[curr_client_id].client_id = curr_client_id;

        if(clients[curr_client_id].client_fd < 0){
            if(has_interrupt()){
                break;
            }
            acceptance_exception();
        }else{
            /*
             * if client_fd is valid,
             * then start new thread for handling that client
             * initialise with no queued work
             */
            clients[curr_client_id].num_work_queued = 0;
            pthread_create(client_threads + curr_client_id,
                NULL,
                server->thread_handler,
                clients + curr_client_id);
            pthread_detach(client_threads[curr_client_id]);
            
            /*
             * incread the curr_id,
             * and try to find a client id that is not occupied
             */
            while(clients[curr_client_id].client_fd > 0){
                curr_client_id++;
                if(!(curr_client_id < MAX_CONNECTION)){
                    curr_client_id = 0;
                }
            }
        }
    }
}

/*****************************************************************************/
/*
 * Helper functions
 */

static void socker_exception(){
    perror("ERROR opening socket");
    general_log_printf(LOG_BASIC, "ERROR", "cannot open socket");
    exit(EXIT_FAILURE);
}

static void port_exception(int portno){
    perror("ERROR unable to use port");
    general_log_printf(LOG_BASIC, "ERROR", "unable to use port %d", portno);
    exit(EXIT_FAILURE);
}

static void acceptance_exception(){
    perror("ERROR on accepting");
    general_log_printf(LOG_BASIC, "WARN", "error on accepting client");
}
