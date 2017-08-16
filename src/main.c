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
#include <signal.h>
#include "constants.h"
#include "server.h"
#include "signal_handler.h"
#include "request_handler.h"
#include "logger.h"

void register_log();

int main(int argc, char *argv[]){
	/*
	 * registering log, so that we can use anywhere
	 */
	register_log();

	general_log_printf(LOG_BASIC, "INFO", "starting server...");
	signal(SIGINT, interrupt_handler);

	if (argc < 2) {
		general_log_printf(LOG_BASIC, "ERRO", "no port provided");
		exit(EXIT_FAILURE);
	}

	int portno = atoi(argv[1]);

	server_t server =
		init_server(AF_INET, SOCK_STREAM, 0, AF_INET, INADDR_ANY, portno);
	general_log_printf(LOG_BASIC,
		"INFO", "server init successfully, listen to port %d", portno);

	register_server_to_interrupt(server);

	register_handler(&server, &thread_handler);

	server_listen(&server);
	return 0;
}

void register_log(){
	FILE *fp;
	fp = fopen(LOG_FILE_PATH, "w+");

	if(fp == NULL){
		general_log_printf(LOG_BASIC,
			"ERROR", "cannot create access/write log file");
		exit(EXIT_FAILURE);
	}

	register_log_file(fp);

	register_log_level(LOG_LEVEL);
}
