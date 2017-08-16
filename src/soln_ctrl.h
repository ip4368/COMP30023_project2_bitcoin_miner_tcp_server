/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Implementing soln message controller,                                 */
/*     this is sticking to interface in message.h                            */
/*                                                                           */
/*****************************************************************************/

#include "message.h"

#ifndef SOLN_CTRL

#define SOLN_CTRL

/*
 * used for the payload void pointer in message.h
 */
typedef struct{
	int alpha, beta;
	uint64_t nonce;
	BYTE *seed;
}soln_t;

/*
 * just the interface functions
 */
void soln_payload_parser(char *buffer, message_t *);

void soln_handler(data_t *, message_t);

#endif
