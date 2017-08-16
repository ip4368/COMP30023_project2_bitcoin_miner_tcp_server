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
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "alloc.h"
#include "constants.h"
#include "logger.h"

#define CLIENT_LOG_HEADER "%17s | %15s | client id %d | %5s: "
#define GENERAL_LOG_HEADER "%17s | %5s: "
#define SESSION_MAKS 0x000000FFU
#define FIRST_SESSION_SHIFT 0
#define SECOND_SESSION_SHIFT 8
#define THIRD_SESSION_SHIFT 16
#define FORTH_SESSION_SHIFT 24

pthread_mutex_t mutex_logger_file;
pthread_mutex_t mutex_logger_screen;
pthread_mutex_t mutex_logger_time;

/*****************************************************************************/
/*
 * Helper prototype
 */

static FILE *log_file_handler(FILE *file);
static FILE *get_log_file();
static int log_level_handler(int log_level);
static int get_log_level();
static char *get_time_str();
static char *get_ip_str(uint32_t addr);

/*****************************************************************************/

void register_log_file(FILE *file){
	pthread_mutex_init(&mutex_logger_file, NULL);
	pthread_mutex_init(&mutex_logger_screen, NULL);
	pthread_mutex_init(&mutex_logger_time, NULL);
	log_file_handler(file);
}

void register_log_level(int log_level){
	log_level_handler(log_level);
}

int general_log_printf(int log_level,
		const char *log_type, const char *format, ...){
	/*
	 * mutex is used as we don't want different client's log to mess up
	 */
	int registered_level = get_log_level();
	int written = 0;

	if(registered_level >= log_level){
		FILE *log_file = get_log_file();
		va_list argptr;

		/*
		 * basically next 2 blocks are equiv, but just goes to different file
		 * mutex for each to speed up process, like after logging to one file
		 * and goes onto another one, so previous one can be used again
		 */
		char *time_str = get_time_str();
		#if ON_SCREEN_LOG
		pthread_mutex_lock(&mutex_logger_screen);
		fprintf(stdout, GENERAL_LOG_HEADER,
	    	time_str,
	    	log_type);
	    va_start(argptr, format);
	    vfprintf(stdout, format, argptr);
	    va_end(argptr);
	    fprintf(stdout, "\n");
		fflush(stdout);
		pthread_mutex_unlock(&mutex_logger_screen);
	    #endif

		pthread_mutex_lock(&mutex_logger_file);
	    fprintf(log_file, GENERAL_LOG_HEADER,
	    	time_str,
	    	log_type);
	    va_start(argptr, format);
	    written = vfprintf(log_file, format, argptr);
	    va_end(argptr);
	    fprintf(log_file, "\n");
	    fflush(log_file);
	    pthread_mutex_unlock(&mutex_logger_file);
	    free(time_str);
	}

    return written;
}

int client_log_printf(int log_level, uint32_t addr, int client_id,
		const char *log_type, const char *format, ...){
	/*
	 * mutex is used as we don't want different client's log to mess up
	 */
	int registered_level = get_log_level();
	int written = 0;

	if(registered_level >= log_level){
		FILE *log_file = get_log_file();
		va_list argptr;

		/*
		 * basically next 2 blocks are equiv, but just goes to different file
		 * mutex for each to speed up process, like after logging to one file
		 * and goes onto another one, so previous one can be used again
		 */
		char *time_str = get_time_str();
		#if ON_SCREEN_LOG
		pthread_mutex_lock(&mutex_logger_screen);
		fprintf(stdout, CLIENT_LOG_HEADER,
	    	time_str,
	    	get_ip_str(addr),
	    	client_id,
	    	log_type);
	    va_start(argptr, format);
	    vfprintf(stdout, format, argptr);
	    va_end(argptr);
	    fprintf(stdout, "\n");
		fflush(stdout);
		pthread_mutex_unlock(&mutex_logger_screen);
	    #endif

		pthread_mutex_lock(&mutex_logger_file);
	    fprintf(log_file, CLIENT_LOG_HEADER,
	    	time_str,
	    	get_ip_str(addr),
	    	client_id,
	    	log_type);
	    va_start(argptr, format);
	    written = vfprintf(log_file, format, argptr);
	    va_end(argptr);
	    fprintf(log_file, "\n");
	    fflush(log_file);
	    pthread_mutex_unlock(&mutex_logger_file);
	    free(time_str);
	}

    return written;
}

/*****************************************************************************/
/*
 * Helper functions
 */

static FILE *log_file_handler(FILE *file){
	static FILE *registered_file = NULL;
	if(file != NULL){
		registered_file = file;
	}
	return registered_file;
}

static FILE *get_log_file(){
	return log_file_handler(NULL);
}

static int log_level_handler(int log_level){
	static int level = -1;
	if(log_level != -1){
		level = log_level;
	}
	return level;
}

static int get_log_level(){
	return log_level_handler(-1);
}

static char *get_time_str(){
	static char buffer[20];
	
	time_t rawtime;
	struct tm * timeinfo;

	pthread_mutex_lock(&mutex_logger_time);
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 20, "%x %X", timeinfo);

	char *time_str = safe_malloc(sizeof(*time_str) * strlen(buffer) + 1);
	strcpy(time_str, buffer);

	pthread_mutex_unlock(&mutex_logger_time);

	return time_str;
}

static char *get_ip_str(uint32_t addr){
	char buffer[16];
	int index = 0;

	index += sprintf(buffer + index, "%u.",
		(addr >> FIRST_SESSION_SHIFT) & SESSION_MAKS);
	index += sprintf(buffer + index, "%u.",
		(addr >> SECOND_SESSION_SHIFT) & SESSION_MAKS);
	index += sprintf(buffer + index, "%u.",
		(addr >> THIRD_SESSION_SHIFT) & SESSION_MAKS);
	index += sprintf(buffer + index, "%u",
		(addr >> FORTH_SESSION_SHIFT) & SESSION_MAKS);

	char *ip_str = safe_malloc(sizeof(*ip_str) * strlen(buffer) + 1);
	strcpy(ip_str, buffer);

	return ip_str;
}
