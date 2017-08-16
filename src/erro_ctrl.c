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
#include "erro_ctrl.h"

void erro_payload_parser(char *buffer, message_t *message){
	message->payload = NULL;
	if(buffer[0] == '\0'){
		message->is_valid_payload = 1;
	}else{
		message->is_valid_payload = 0;
	}
}

void erro_handler(data_t *client, message_t message){
	(void) message;
	respond_client(*client, "UNIM");
}

void erro_reserved_for_server(data_t *client){
	respond_client(*client, "ERRO header reserved for server");
}

void erro_invalid_payload(data_t *client){
	respond_client(*client, "ERRO invalid payload");
}

void erro_header_not_found(data_t *client){
	respond_client(*client, "ERRO header is not specified in protocol");
}
