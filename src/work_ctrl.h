/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Implementing work message controller                                  */
/*     this is sticking to interface in message.h                            */
/*                                                                           */
/*****************************************************************************/

#include "message.h"

#ifndef WORK_CTRL

#define WORK_CTRL

/*
 * used for the payload void pointer in message.h
 */
typedef struct{
	int alpha, beta;
	uint64_t start;
	BYTE *seed;
	uint8_t worker_count;
}work_t;

/*
 * just the interface functions
 */
void work_payload_parser(char *buffer, message_t *);

void work_handler(data_t *, message_t);

#endif
