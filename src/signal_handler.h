/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Signal Handler for server                                             */
/*                                                                           */
/*****************************************************************************/

#ifndef IPS_SIGNAL_HANDLE

#define IPS_SIGNAL_HANDLE

#include "server.h"

/*
 * just something that will gracefully termincate the server
 * closing everything properly
 */
void interrupt_handler(int na);
int has_interrupt();
void register_server_to_interrupt(server_t server);

#endif
