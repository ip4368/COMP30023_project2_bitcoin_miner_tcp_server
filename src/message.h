/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     all the things related to messages, including server and client       */
/*     need to add to the #define below to add new message header            */
/*                                                                           */
/*****************************************************************************/

#include "server.h"
#include "uint256.h"

#ifndef MESSAGE

#define MESSAGE

/*
 * need to add more if more message headers,
 * each message header need to have an id,
 * associate with the order in controller
 */
#define NONE 0
#define PING 1
#define PONG 2
#define OKAY 3
#define ERRO 4
#define SOLN 5
#define WORK 6
#define ABRT 7

#define SEED_LEN_BYTES 64
#define ERR_MSG_LEN 41

#define MSG_TYPE_COUNT 7
#define MSG_HEADER_WIDTH 4
#define END_OF_MSG_WIDTH 1

#define CLIENT_MSG 1
#define SERVER_MSG 2
#define BOTH_MSG 0

typedef int message_type_no_t;

/*
 * interface for the message
 */
typedef struct{
	/*
	 * the message type in int, which are the constants above
	 */
	message_type_no_t msg_type;

	/*
	 * this is a boolean that store if the payload is valid (valid in parsing)
	 */
	int is_valid_payload;

	/*
	 * the void pointer to the payload, anything can be assigned and retrieved
	 */
	void *payload;

	/*
	 * the vanilla message as a string from client,
	 * mainly for the request handler to print incoming messages
	 */
	char *vanilla_msg;
}message_t;

/*
 * interface of message handler
 * mainly used in controller.h, look at that!
 */
typedef struct{
	/*
	 * The message header as a string, NEED unique within the system
	 */
	char *msg;

	/*
	 * The type of the message
	 * 1. CLIENT_MSG - should only be sent from client
	 * 2. SERVER_MSG - should only be sent from server
	 * 3. BOTH_MSG   - can be used by both client and server
	 */
	int header_type;

	/*
	 * a function that will give you the buffer of the payload
	 * and put it into the message_t * instance
	 */
	void (*payload_parser)(char *, message_t *);

	/*
	 * the handler to process the actual request
	 * data_t is the data specifying the client
	 */
	void (*handler)(data_t *, message_t);
}msg_header_t;

#endif
