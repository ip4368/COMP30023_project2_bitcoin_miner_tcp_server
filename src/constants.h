/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     file contain all tunable constants                                    */
/*                                                                           */
/*****************************************************************************/

#include "logger.h"

#ifndef CONSTANTS

#define CONSTANTS

/*
 * The file including directory of where the log file should be placed
 */
#define LOG_FILE_PATH "./log.txt"

/*
 * LOG_LEVEL can be set to 
 * 1. LOG_BASIC
 *     This provides basic infomation of the server, including
 *         server startup and termination
 *         incomming and outgoing messages
 * 2. LOG_EXTRA
 *     This provides some debugging info includes
 *         client connect and disconnect
 *         message matched type
 *         job queuing and removing
 *         worker spawning while work message received
 */
#define LOG_LEVEL LOG_BASIC

/*
 * log file log is always on, but you can choose to log on screen or not
 */
#define ON_SCREEN_LOG 1

/*
 * the max connection that this server should handle
 */
#define MAX_CONNECTION 1000

/*
 * the number of work messages that can queue up
 */
#define MAX_WORK_QUEUE 10

#endif
