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
#include "alloc.h"
#include "controller.h"
#include "logger.h"
#include "work_ctrl.h"
#include "work_daemon.h"

#define ALPHA_MASK 0xFF000000
#define ALPHA_SHIFT 24
#define BETA_MASK 0x00FFFFFF
#define BETA_SHIFT 0

#define MAX_DIFF_LEN 8
#define MAX_SOLN_LEN 16
#define HASH_LEN 32
#define SEED_LEN 32
#define PAD_LEN 8
#define MAX_WORKER_LEN 2

/*****************************************************************************/
/*
 * Helper prototype
 */

static int is_hex_char(char ch);

/*****************************************************************************/

void work_payload_parser(char *buffer, message_t *message){
	work_t *data = safe_malloc(sizeof(*data));

	int offset = 0;

	if(buffer[offset] != ' '){
		message->is_valid_payload = 0;
		return;
	}

	offset++;

	int i;
	for(i=0; ; i++){
		char ch = buffer[i + offset];
		if(ch == '\0'){
			message->is_valid_payload = 0;
			return;
		}
		if(ch == ' '){
			break;
		}
		if((!is_hex_char(ch)) || i == MAX_DIFF_LEN + 1){
			message->is_valid_payload = 0;
			return;
		}
	}
	if(i != 8){
		message->is_valid_payload = 0;
		return;
	}

	uint32_t temp;
	sscanf(buffer + offset, "%8x ", &temp);
	data->alpha = (temp & ALPHA_MASK) >> ALPHA_SHIFT;
	data->beta = (temp & BETA_MASK) >> BETA_SHIFT;

	offset += i + 1;

	BYTE *seed = safe_malloc(sizeof(*seed) * SEED_LEN);
	for(i=0; i<SEED_LEN; i++){
		if(!(is_hex_char(buffer[offset + i])
				&& is_hex_char(buffer[offset + i + 1]))){
			message->is_valid_payload = 0;
			return;
		}
		sscanf(buffer + offset + i * 2, "%2x", &temp);
		seed[i] = (BYTE)temp;
	}
	data->seed = seed;

	offset += SEED_LEN * 2;

	if(buffer[offset] != ' '){
		message->is_valid_payload = 0;
		return;
	}

	offset++;

	for(i=0; ; i++){
		char ch = buffer[i + offset];
		if(ch == '\0'){
			message->is_valid_payload = 0;
			return;
		}
		if(ch == ' '){
			break;
		}
		if((!is_hex_char(ch)) || i == MAX_SOLN_LEN + 1){
			message->is_valid_payload = 0;
			return;
		}
	}
	if(i != MAX_SOLN_LEN){
		message->is_valid_payload = 0;
		return;
	}
	sscanf(buffer + offset, "%16llx", (long long unsigned int *)&data->start);

	offset += MAX_SOLN_LEN;

	if(buffer[offset] != ' '){
		message->is_valid_payload = 0;
		return;
	}

	offset++;

	for(i=0; ; i++){
		char ch = buffer[i + offset];
		if(ch == '\0'){
			break;
		}
		if((!is_hex_char(ch)) || i == MAX_WORKER_LEN + 1){
			message->is_valid_payload = 0;
			return;
		}
	}
	if(i != MAX_WORKER_LEN){
		message->is_valid_payload = 0;
		return;
	}
	int worker_count;
	sscanf(buffer + offset, "%2x", &worker_count);
	data->worker_count = worker_count;

	message->payload = data;
	message->is_valid_payload = 1;
}

void work_handler(data_t *client, message_t message){
	work_t *data = (work_t *)message.payload;
	/*
	 * try to add job, if not success, it will return 0
	 */
	if(!add_to_daemon(*data, *client)){
		respond_client(*client, "ERRO queue is full, please try later");
	}
}

/*****************************************************************************/
/*
 * Helper functions
 */

static int is_hex_char(char ch){
	return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f');
}
