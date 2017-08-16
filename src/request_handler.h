/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     main handler for requests                                             */
/*                                                                           */
/*****************************************************************************/

#include "server.h"

#ifndef IPS_REQ_HANDLER

#define IPS_REQ_HANDLER

#define BUFFER_LEN 255

/*
 * basically new thread spin up on every connect,
 * this is the function that handle one client
 * will terminate when client disconnect
 */
void* thread_handler(void *data);

/*
 * the standardise way to repond to client
 */
void respond_client(data_t client, char *response);

#endif
