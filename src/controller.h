/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     controller interface for messages                                     */
/*                                                                           */
/*****************************************************************************/

#include "message.h"
#include "unknown_ctrl.h"
#include "ping_ctrl.h"
#include "pong_ctrl.h"
#include "okay_ctrl.h"
#include "erro_ctrl.h"
#include "soln_ctrl.h"
#include "work_ctrl.h"
#include "abrt_ctrl.h"

#ifndef CONTROLLER

#define CONTROLLER

typedef struct{
	void (*payload_parser)(char *, message_t *);
	void (*handler)(data_t *, message_t);
}controller_t;

/*
 * All the messages type should be specified here
 * {message header, client/server/both,
 *  parser function for payload, handler function to respond to client}
 */
static const msg_header_t MSG_HEADER[] = 
	{
		{"UNKNOWN", CLIENT_MSG, &unknown_payload_parser, &unknown_handler},
		{"PING", CLIENT_MSG, &ping_payload_parser, &ping_handler},
		{"PONG", SERVER_MSG, &pong_payload_parser, &pong_handler},
		{"OKAY", SERVER_MSG, &okay_payload_parser, &okay_handler},
		{"ERRO", SERVER_MSG, &erro_payload_parser, &erro_handler},
		{"SOLN", BOTH_MSG, &soln_payload_parser, &soln_handler},
		{"WORK", CLIENT_MSG, &work_payload_parser, &work_handler},
		{"ABRT", CLIENT_MSG, &abrt_payload_parser, &abrt_handler},
	};

#endif
