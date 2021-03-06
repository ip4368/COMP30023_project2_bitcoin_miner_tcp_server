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
#include "okay_ctrl.h"

void okay_payload_parser(char *buffer, message_t *message){
	message->payload = NULL;
	if(buffer[0] == '\0'){
		message->is_valid_payload = 1;
	}else{
		message->is_valid_payload = 0;
	}
}

void okay_handler(data_t *client, message_t message){
	(void) message;
	respond_client(*client, "UNIM");
}
