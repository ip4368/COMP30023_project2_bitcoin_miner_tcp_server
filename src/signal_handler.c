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
#include <sys/socket.h>
#include <unistd.h>
#include "signal_handler.h"
#include "server.h"
#include "logger.h"

/*****************************************************************************/
/*
 * Helper prototype
 */

static int interrupt(int term);
static server_t get_registered(int update, server_t server);

/*****************************************************************************/

void interrupt_handler(int na){
    (void)na;  // suppress warning
    general_log_printf(LOG_BASIC,
                "INFO", "signal interrupt received -- cleaning up");
    server_t server;
    server = get_registered(0, server);
    close(server.sock_fd);
    interrupt(1);
}

int has_interrupt(){
    return interrupt(0);
}

void register_server_to_interrupt(server_t server){
    get_registered(1, server);
}

/*****************************************************************************/
/*
 * Helper functions
 */

static int interrupt(int term){
    static int interrupted = 0;
    if(term){
        interrupted = 1;
    }
    return interrupted;
}

static server_t get_registered(int update, server_t server){
    static server_t registered;
    if(update){
        registered = server;
    }
    return registered;
}
