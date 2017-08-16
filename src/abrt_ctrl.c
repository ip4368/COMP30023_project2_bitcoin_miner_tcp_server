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
#include "request_handler.h"
#include "controller.h"
#include "logger.h"
#include "abrt_ctrl.h"
#include "work_daemon.h"

void abrt_payload_parser(char *buffer, message_t *message){
	message->payload = NULL;
	if(buffer[0] == '\0'){
		message->is_valid_payload = 1;
	}else{
		message->is_valid_payload = 0;
	}
}

void abrt_handler(data_t *client, message_t message){
	(void)message;
	int client_id = client->client_id;
	remove_work_for_client(client_id);
	respond_client(*client, "OKAY");
}
