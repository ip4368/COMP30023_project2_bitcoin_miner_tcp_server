/*****************************************************************************/
/*                                                                           */
/*          This is a submission for assignment 2 of                         */
/*          COMP30023 Computer Systems                                       */
/*                                                                           */
/*          Author     : Shing Sheung Daniel Ip                              */
/*          Student ID : 723508                                              */
/*          Finish Time: 26th May 2017 10:46                                 */
/*                                                                           */
/*     component to start a daemon like things by starting start_work_daemon */
/*     start_work_daemon will start working while something being added      */
/*     but only one work message will be processed (the head of the queue)   */
/*     this is just sticking to assignment requirement                       */
/*                                                                           */
/*****************************************************************************/

/*
 * The server should start one daemon thread, monotoring the work queue
 * and also handle the process of spawning child thread to handle the message
 */

/*
 * this provide semaphore support for both linux and mac,
 * no other platform has been tested
 */
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#include <stdint.h>
#include <pthread.h>
#include "work_ctrl.h"
#include "constants.h"
#include "queue.h"

#ifndef WORK_DAEMON

#define WORK_DAEMON

enum state_t{
	queued, active, terminating, 
};

typedef struct{
	#ifdef __APPLE__
    dispatch_semaphore_t has_queued_sem;
	#else
    sem_t has_queued_sem;
	#endif
	pthread_mutex_t num_work_lock;
	pthread_mutex_t state_lock;
	int num_work_in_queue;
	Queue work_queue;
}work_daemon_t;

typedef struct{
	work_t work;
	uint64_t curr_nonce;
	pthread_mutex_t get_job_lock;
	pthread_mutex_t respond_lock;
	data_t client;
	enum state_t state;
}work_item_t;

/*
 * initialise the daemon, please call before using anything else in this file
 */
void init_work_daemon();

/*
 * return 1 if successfully added to queue,
 * might not be successful as could have too many works queued
 */
int add_to_daemon(work_t work, data_t client);

/*
 * just remove all the work from the client with client id
 */
void remove_work_for_client(int client_id);

/*
 * Just like getter and setter
 */
void set_daemon(work_daemon_t *daemon);
work_daemon_t *get_daemon();

/*
 * call this with pthread_create in main / server
 * a daemon thread that will dispatch the job to worker
 */
void *start_work_daemon(void *data);

#endif
