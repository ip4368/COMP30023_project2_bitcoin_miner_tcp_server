/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Implementing a controller that tackle unknown message                 */
/*     which mean this is the default controller used                        */
/*     if the header doens't match                                           */
/*     this is sticking to interface in message.h                            */
/*                                                                           */
/*****************************************************************************/

#include "message.h"

#ifndef UNKNOWN_CTRL

#define UNKNOWN_CTRL

/*
 * used for the payload void pointer in message.h
 * which will not be used
 */
typedef struct{
	// nothing need to be store
}unknown_t;

/*
 * just the interface functions
 */
void unknown_payload_parser(char *buffer, message_t *);

void unknown_handler(data_t *, message_t);

#endif
