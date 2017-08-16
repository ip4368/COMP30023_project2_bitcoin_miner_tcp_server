/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     parsing component for client input                                    */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "message.h"

#ifndef PARSER

#define PARSER

/*
 * this handle left-over buffer as well,
 * and a messages array for all completed messages
 */
typedef struct{
	message_t *messages;
	uint32_t message_count;
	char *remain_buffer;
	uint32_t remain_buffer_len;
}parse_t;

/*
 * This function parse one line of the client message
 * will call the parser function in the controller for message as well
 */
parse_t parse(char *buffer, int len,
		char *remain_buffer, uint32_t remain_buffer_len);

#endif
