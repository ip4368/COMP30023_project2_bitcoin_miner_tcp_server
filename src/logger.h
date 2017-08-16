/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     logger for whole program, support multithreading log                  */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdint.h>

#ifndef LOGGER

#define LOGGER

/*
 * The log level that can be chosen, look at constants.c to configure log level
 */
#define LOG_BASIC 0
#define LOG_EXTRA 1

/*
 * a file pointer for the log file,
 * if don't want to log to file, but only on screen
 * this can be set to stdout/stderr, then turn off original on screen log
 */
void register_log_file(FILE *file);

/*
 * specify the server log level
 */
void register_log_level(int log_level);

/*
 * logger for server general purpose that not really related to client
 */
int general_log_printf(int log_level,
		const char *log_type, const char *format, ...);

/*
 * logger for things related to client, specify the client add and id
 */
int client_log_printf(int log_level, uint32_t addr, int client_id,
	const char *log_type, const char *format, ...);

#endif
