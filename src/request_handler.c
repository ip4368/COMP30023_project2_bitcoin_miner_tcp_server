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
#include <unistd.h>
#include <stdint.h>
#include "request_handler.h"
#include "message.h"
#include "parser.h"
#include "server.h"
#include "logger.h"
#include "controller.h"
#include "work_daemon.h"

#define SERVER_ADDR 0

/*****************************************************************************/
/*
 * Helper prototype
 */

static void read_exception();
static void clean_up_connection(data_t *data);
static void lost_connection_exception(uint32_t addr, int fd, int id);

/*****************************************************************************/

void* thread_handler(void *data){
	char buffer[BUFFER_LEN + 1];

	data_t *d = (data_t *)data;
	int client_fd = d->client_fd;
	int client_id = d->client_id;
	uint32_t addr = d->addr.sin_addr.s_addr;

	client_log_printf(LOG_EXTRA, addr, client_id,
		"INFO", "connected from client");

	int n;
	char *remain_buffer = NULL;
	uint32_t remain_buffer_len = 0;

	while(1){
		n = read(client_fd, buffer, BUFFER_LEN);

		if(n == 0){
			/*
			 * if read equal to 0, no byte is read in (but successfully read),
			 * therefore, connection lost!
			 */
			clean_up_connection(d);
			lost_connection_exception(addr, client_fd, client_id);
			break;
		}else if(n < 0){
			read_exception(client_id);
		}else{
			parse_t parsed =
				parse(buffer, n, remain_buffer, remain_buffer_len);
			remain_buffer = parsed.remain_buffer;
			remain_buffer_len = parsed.remain_buffer_len;

			uint32_t i;
			for(i=0; i<parsed.message_count; i++){
				client_log_printf(LOG_BASIC, addr, client_id,
					"IMSG", parsed.messages[i].vanilla_msg);
				client_log_printf(LOG_EXTRA, addr, client_id,
					"INFO", "matched with type no %d with payload valid %d",
					parsed.messages[i].msg_type,
					parsed.messages[i].is_valid_payload);

				/*
				 * dispatch the actual command
				 */
				//(*MSG_HEADER[parsed.messages[i].msg_type].handler)
				//	(d, parsed.messages[i]);
				int h_type = 
					MSG_HEADER[parsed.messages[i].msg_type].header_type;
				if(h_type == SERVER_MSG){
					erro_reserved_for_server(d);
				}else if(!parsed.messages[i].is_valid_payload){
					erro_invalid_payload(d);
				}else{
					if(*MSG_HEADER[parsed.messages[i].msg_type].handler
							!= NULL){
						(*MSG_HEADER[parsed.messages[i].msg_type].handler)
							(d, parsed.messages[i]);
					}else{
						general_log_printf(LOG_EXTRA,
							"WARN", "Handler for type %d not found",
							parsed.messages[i].msg_type);
					}
				}
			}
		}
	}
	return NULL;
}

void respond_client(data_t client, char *response){
	int id = client.client_id;

	client_log_printf(LOG_BASIC, SERVER_ADDR, id, "OMSG", "%s", response);
	dprintf(client.client_fd, "%s\r\n", response);
}

/*****************************************************************************/
/*
 * Helper functions
 */

static void read_exception(uint32_t addr, int id){
	client_log_printf(LOG_EXTRA, addr, id,
		"ERROR", "failed reading from socket");
}

static void clean_up_connection(data_t *data){
	/*
	 * reset so it can be reuse
	 */
	data->client_fd = -1;
}

static void lost_connection_exception(uint32_t addr, int fd, int id){
	remove_work_for_client(id);
	client_log_printf(LOG_EXTRA, addr, id, "INFO", "disconnected from client");
	/*
	 * properly close the file descriptor
	 */
	close(fd);
}
