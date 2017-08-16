/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Implementing ping message controller                                  */
/*     this is sticking to interface in message.h                            */
/*                                                                           */
/*****************************************************************************/

#include "message.h"

#ifndef PING_CTRL

#define PING_CTRL

/*
 * used for the payload void pointer in message.h
 * which will not be used
 */
typedef struct{
	// nothing need to be store
}ping_t;

/*
 * just the interface functions
 */
void ping_payload_parser(char *buffer, message_t *);

void ping_handler(data_t *, message_t);

#endif
