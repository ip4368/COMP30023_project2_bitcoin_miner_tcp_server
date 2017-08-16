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
#include <string.h>
#include "parser.h"
#include "alloc.h"
#include "message.h"
#include "controller.h"
#include "request_handler.h"
#include "logger.h"

#define INIT_ARR_SIZE 100
#define INIT_LENGTH 255

/*****************************************************************************/
/*
 * Helper prototype
 */

static int parse_type(char *buffer, message_t *message);

/*****************************************************************************/

parse_t parse(char *buffer, int len,
		char *remain_buffer, uint32_t remain_buffer_len){
	uint32_t pos = 0;
	char **temp_msgs;
	uint32_t msgs_count = 0, msgs_max = INIT_ARR_SIZE;
	temp_msgs = safe_malloc(sizeof(*temp_msgs) * msgs_max);
	
	uint32_t msg_len = 0, msg_max = INIT_LENGTH;
	if(remain_buffer_len >= msg_max){
		msg_max = remain_buffer_len * 2;
	}
	temp_msgs[msgs_count] =
		safe_malloc(sizeof(*(temp_msgs[msgs_count])) * msg_max + 1);
	if(remain_buffer_len){
		strcpy(temp_msgs[msgs_count], remain_buffer);
		msg_len = remain_buffer_len;
	}

	/*
	 * special case, last char in remain buffer is \r,
	 * first char in buffer is \n, the loop below will not be able to handle
	 */
	if(len
			&& remain_buffer_len
			&& temp_msgs[msgs_count][msg_len - 1] == '\r'
			&& buffer[0] == '\n'){
		temp_msgs[msgs_count][msg_len - 1] = '\0';

		/*
		 * move onto next msg
		 */
		msgs_count++;
		if(msgs_count == msgs_max){
			msgs_max *= 2;
			temp_msgs = 
				safe_realloc(temp_msgs, sizeof(*temp_msgs) * msgs_max);
		}

		/*
		 * restart the msg length
		 * and malloc for new msg
		 */
		msg_len = 0, msg_max = INIT_LENGTH;
		temp_msgs[msgs_count] =
			safe_malloc(sizeof(*(temp_msgs[msgs_count])) * msg_max + 1);

		/*
		 * skip the first \n char
		 */
		pos++;
	}

	while(pos < (uint32_t)len){
		if(buffer[pos] == '\r'
				&& pos + 1 < (uint32_t)len
				&& buffer[pos + 1] == '\n'){
			/*
			 * Finish of one command
			 * add null byte and move on
			 */
			temp_msgs[msgs_count][msg_len] = '\0';

			/*
			 * move onto next msg
			 */
			msgs_count++;
			if(msgs_count == msgs_max){
				msgs_max *= 2;
				temp_msgs = 
					safe_realloc(temp_msgs, sizeof(*temp_msgs) * msgs_max);
			}

			/*
			 * restart the msg length
			 * and malloc for new msg
			 */
			msg_len = 0, msg_max = INIT_LENGTH;
			temp_msgs[msgs_count] =
				safe_malloc(sizeof(*(temp_msgs[msgs_count])) * msg_max + 1);
			pos += 2;
			continue;
		}
		/*
		 * actual copy take place
		 */
		temp_msgs[msgs_count][msg_len] = buffer[pos];
		
		pos++;
		msg_len++;

		/*
		 * if msg is very long, realloc
		 */
		if(msg_len == msg_max){
			msg_max *= 2;
			temp_msgs[msgs_count] = safe_realloc(temp_msgs[msgs_count],
				sizeof(*(temp_msgs[msgs_count])) * msg_max + 1);
		}
	}

	parse_t res;

	/*
	 * left over buffer
	 */
	if(msg_len){
		temp_msgs[msgs_count][msg_len] = '\0';
		res.remain_buffer_len = msg_len;
		res.remain_buffer = 
			safe_malloc(sizeof(*res.remain_buffer) * msg_len + 1);
		strcpy(res.remain_buffer, temp_msgs[msgs_count]);
	}else{
		res.remain_buffer_len = 0;
		res.remain_buffer = NULL;
	}

	/*
	 * malloc same amount of messages we know
	 */
	res.message_count = msgs_count;
	res.messages = safe_malloc(sizeof(*res.messages) * res.message_count);
	uint32_t i;
	for(i=0; i<res.message_count; i++){
		int used_len = parse_type(temp_msgs[i], res.messages + i);
		res.messages[i].vanilla_msg = temp_msgs[i];
		/*
		 * after parsing the header, call the function pointer to parse payload
		 */
		if(*MSG_HEADER[res.messages[i].msg_type].payload_parser != NULL){
			(*MSG_HEADER[res.messages[i].msg_type].payload_parser)
				(temp_msgs[i] + used_len, res.messages + i);
		}else{
			general_log_printf(LOG_EXTRA,
				"WARN", "Parser for type %d not found",
				res.messages[i].msg_type);
		}
	}

	return res;
}

/*****************************************************************************/
/*
 * Helper functions
 */

static int parse_type(char *buffer, message_t *message){
	char *header = safe_malloc(sizeof(*header) * strlen(buffer) + 1);
	int header_len;
	for(header_len=0; 
			buffer[header_len] != '\0' && buffer[header_len] != ' ';
			header_len++){
		header[header_len] = buffer[header_len];
	}
	header[header_len] = '\0';

	if(header_len < MSG_HEADER_WIDTH){
		message->msg_type = NONE;
		return header_len;
	}

	/*
	 * Finding if this which header is it
	 */
	message->msg_type = NONE;
	int type_no;
	for(type_no=NONE + 1; type_no<MSG_TYPE_COUNT + 1; type_no++){
		if(strcmp(header, MSG_HEADER[type_no].msg) == 0){
			/*
			 * found and break
			 */
			message->msg_type = type_no;
			break;
		}
	}
	return header_len;
}
