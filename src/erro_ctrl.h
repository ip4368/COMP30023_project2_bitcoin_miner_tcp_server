/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     Implementing erro message controller                                  */
/*     this is sticking to interface in message.h                            */
/*                                                                           */
/*****************************************************************************/

#include "message.h"

#ifndef ERRO_CTRL

#define ERRO_CTRL

/*
 * used for the payload void pointer in message.h
 */
typedef struct{
	char err_message[ERR_MSG_LEN];
}erro_t;

/*
 * just the interface functions
 */
void erro_payload_parser(char *buffer, message_t *);

void erro_handler(data_t *, message_t);

/*
 * some error functions that will basically send back error to client
 */
void erro_reserved_for_server(data_t *client);
void erro_invalid_payload(data_t *client);
void erro_header_not_found(data_t *client);

#endif
