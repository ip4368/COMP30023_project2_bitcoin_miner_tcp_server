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
#include "soln_ctrl.h"

#include "hashcash.h"
#include "uint256.h"
#include "sha256.h"
#include "solution.h"

#define ALPHA_MASK 0xFF000000
#define ALPHA_SHIFT 24
#define BETA_MASK 0x00FFFFFF
#define BETA_SHIFT 0

#define MAX_DIFF_LEN 8
#define MAX_SOLN_LEN 16

#ifndef SEED_LEN
#define SEED_LEN 32
#endif

/*****************************************************************************/
/*
 * Helper prototype
 */

static int is_hex_char(char ch);

/*****************************************************************************/

void soln_payload_parser(char *buffer, message_t *message){
	soln_t *data = safe_malloc(sizeof(*data));

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
			break;
		}
		if((!is_hex_char(ch)) || i == MAX_SOLN_LEN + 1){
			message->is_valid_payload = 0;
			return;
		}
	}
	if(i != 16){
		message->is_valid_payload = 0;
		return;
	}

	sscanf(buffer + offset, "%16llx", (long long unsigned int *)&data->nonce);

	message->payload = data;
	message->is_valid_payload = 1;
}

void soln_handler(data_t *client, message_t message){
	soln_t *data = (soln_t *)message.payload;

	BYTE target[32];
	init_target(target, data->alpha, data->beta);

	SHA256_CTX first_hash_ctx = hashed_seed(data->seed);

	if(is_valid_solution(target, data->nonce, first_hash_ctx)){
		respond_client(*client, "OKAY");
		return;
	}
	respond_client(*client, "ERRO not a valid proof-of-work solution");
}

/*****************************************************************************/
/*
 * Helper functions
 */

static int is_hex_char(char ch){
	return ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f');
}
