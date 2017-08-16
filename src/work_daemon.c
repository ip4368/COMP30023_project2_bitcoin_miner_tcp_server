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

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#include <pthread.h>
#include "constants.h"
#include "alloc.h"
#include "work_daemon.h"
#include "work_worker.h"
#include "logger.h"

/*****************************************************************************/
/*
 * Helper prototype
 */

static work_daemon_t *daemon_handler(work_daemon_t *daemon);

/*****************************************************************************/

void init_work_daemon(){
	work_daemon_t *work_daemon = get_daemon();

#ifdef __APPLE__
    dispatch_semaphore_t *sem = &work_daemon->has_queued_sem;

    *sem = dispatch_semaphore_create(0);
#else
    /*
	 * (&work_daemon->queued,
	 *  only share between thread,
	 *  no queued work)
	 */
    sem_init(&work_daemon->has_queued_sem, 0, 0);
#endif
	pthread_mutex_init(&work_daemon->num_work_lock, NULL);
	pthread_mutex_init(&work_daemon->state_lock, NULL);

	work_daemon->num_work_in_queue = 0;
	work_daemon->work_queue = newQueue();

	/*
	 * Create the thread that do the daemon
	 */
	pthread_t work_daemon_thread;
	pthread_create(&work_daemon_thread,
        NULL,
        &start_work_daemon,
        NULL);
}

int add_to_daemon(work_t work, data_t client){
	work_daemon_t *work_daemon = get_daemon();

	pthread_mutex_lock(&work_daemon->num_work_lock);

	/*
	 * The queue is FULL!!!
	 */
	if(work_daemon->num_work_in_queue == MAX_WORK_QUEUE){
		pthread_mutex_unlock(&work_daemon->num_work_lock);
		return 0;
	}
	work_item_t *work_item = safe_malloc(sizeof(*work_item));
	work_item->work = work;
	work_item->client = client;

	work_item->state = queued;

	pthread_mutex_init(&work_item->get_job_lock, NULL);
	pthread_mutex_init(&work_item->respond_lock, NULL);

	work_item->curr_nonce = work.start;

	enqueue(work_daemon->work_queue, work_item);

	/*
	 * increase semaphore stating that there are more queued job
	 * NEED TO AFTER ENQUEUE
	 */
#ifdef __APPLE__
    dispatch_semaphore_signal(work_daemon->has_queued_sem);
#else
    sem_post(&work_daemon->has_queued_sem);
#endif

	work_daemon->num_work_in_queue++;

	pthread_mutex_unlock(&work_daemon->num_work_lock);
	general_log_printf(LOG_EXTRA, "INFO", "add job to queue");

	return 1;
}

void remove_work_for_client(int client_id){
	work_daemon_t *work_daemon = get_daemon();

	pthread_mutex_lock(&work_daemon->num_work_lock);

	if(queue_is_empty(work_daemon->work_queue)){
		// nothing in queue, don't do anything
		pthread_mutex_unlock(&work_daemon->num_work_lock);
		return;
	}

	QueueNode curr = work_daemon->work_queue->tail;
	while(curr != NULL){
		work_item_t *work_item = (work_item_t *)curr->data;
		QueueNode previous = curr->previous;
		/*
		 * DO NOT REMOVE OTHER CLIENT QUEUED WORK
		 */
		if(work_item->client.client_id == client_id){
			/*
			 * No matter what the job state is, they should be terminating now
			 * still need to lock for state changing
			 */
			pthread_mutex_lock(&work_daemon->state_lock);
			work_item->state = terminating;
			pthread_mutex_unlock(&work_daemon->state_lock);

			/*
			 * remove this node from the queue, but should not free data
			 * the worker thread need to know the terminating state
			 * (need to add terminated worker count,
			 * then last worker terminated need to free the data)
			 *
			 */
			queue_remove(work_daemon->work_queue, curr);
			work_daemon->num_work_in_queue--;

			/*
			 * Only decrease semaphore if the tast is not has_queued_sem
			 */
			pthread_mutex_lock(&work_daemon->state_lock);
			if(work_item->state == queued){
			#ifdef __APPLE__
			    dispatch_semaphore_wait(work_daemon->has_queued_sem,
			    	DISPATCH_TIME_FOREVER);
			#else
			    int r;
			    do{
		            r = sem_wait(&work_daemon->has_queued_sem);
			    } while (r == -1);
			#endif
			}
			pthread_mutex_unlock(&work_daemon->state_lock);
		}
		curr = previous;
	}
	pthread_mutex_unlock(&work_daemon->num_work_lock);
}

void set_daemon(work_daemon_t *daemon){
	daemon_handler(daemon);
}

work_daemon_t *get_daemon(){
	return daemon_handler(NULL);
}

void *start_work_daemon(void *data){
	// suppress warning
	(void) data;

	work_daemon_t *work_daemon = get_daemon();

	/*
	 * loop for consuming the work in queue
	 */
	while(1){
		general_log_printf(LOG_EXTRA, "INFO", "waiting for job");
		pthread_mutex_lock(&work_daemon->state_lock);
	#ifdef __APPLE__
	    dispatch_semaphore_wait(work_daemon->has_queued_sem,
	    	DISPATCH_TIME_FOREVER);
	#else
	    int r;
	    do{
            r = sem_wait(&work_daemon->has_queued_sem);
	    } while (r == -1);
	#endif
		//sem_wait(&work_daemon->has_queued_sem);
		/*
		 * decrease semaphore and change state
		 */
		QueueNode head = work_daemon->work_queue->head;
		if(head == NULL){
			pthread_mutex_unlock(&work_daemon->state_lock);
			continue;
		}
		work_item_t *work_item = (work_item_t *)head->data;
		/*
		 * Do nothing if it is not queued
		 */
		if(work_item->state != queued){
			pthread_mutex_unlock(&work_daemon->state_lock);
			continue;
		}
		work_item->state = active;
		pthread_mutex_unlock(&work_daemon->state_lock);

		general_log_printf(LOG_EXTRA, "INFO", "grab job from queue successfully");

		int worker_count = work_item->work.worker_count;
		
		/*
		 * Start spawning thread to work on the work message
		 */
		pthread_t *worker_theads = 
        	safe_malloc(sizeof(*worker_theads) * worker_count);
    	uint8_t i;

    	general_log_printf(LOG_EXTRA, "INFO", "spawning workers");
		for(i=0; i<worker_count; i++){
			pthread_create(worker_theads + i,
				NULL,
				&work_worker,
				work_item);
		}
		
		for(i=0; i<worker_count; i++){
			pthread_join(worker_theads[i], NULL);
		}
		general_log_printf(LOG_EXTRA, "INFO", "workers joined");

		/*
		 * if the queue is empty, the upper thing is going to lock it
		 * (potential bug, they don't remove, I remove then how to solve?)
		 */
		pthread_mutex_lock(&work_daemon->num_work_lock);
		/*
		 * Only try to remove when the head is still the head
		 */
		if(work_daemon->work_queue->head == head){
			queue_remove(work_daemon->work_queue, head);
			work_daemon->num_work_in_queue--;
		}
		pthread_mutex_unlock(&work_daemon->num_work_lock);
	}
	
	return NULL;
}

/*****************************************************************************/
/*
 * Helper functions
 */

static work_daemon_t *daemon_handler(work_daemon_t *daemon){
	static work_daemon_t *daemon_ob;
	if(daemon != NULL){
		daemon_ob = daemon;
	}
	return daemon_ob;
}
