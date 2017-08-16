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

#include <pthread.h>
#include <stdint.h>
#include "work_worker.h"
#include "work_daemon.h"
#include "solution.h"
#include "hashcash.h"
#include "logger.h"
#include "request_handler.h"

#define SOL_MSG_LEN 95

void *work_worker(void *data){
	general_log_printf(LOG_EXTRA, "INFO", "worker spawned");
	work_item_t *work_item = (work_item_t *)data;

	BYTE target[32];
	init_target(target, work_item->work.alpha, work_item->work.beta);

	SHA256_CTX first_hash_ctx = hashed_seed(work_item->work.seed);

	uint64_t curr_nonce;
	/*
	 * keep looping until solution found, or state become terminating
	 */
	while(1){
		if(work_item->state == terminating){
			break;
		}
		/*
		 * aquire the lock to find which number to check
		 */
		pthread_mutex_lock(&work_item->get_job_lock);
		curr_nonce = work_item->curr_nonce;
		work_item->curr_nonce++;
		pthread_mutex_unlock(&work_item->get_job_lock);

		if(is_valid_solution(target, curr_nonce, first_hash_ctx)){
			/*
			 * construct the string
			 */
			char soln_msg[SOL_MSG_LEN + 1];
			int curr_index = 0;
			sprintf(soln_msg + curr_index, "%s", "SOLN ");
			curr_index += 5;
			sprintf(soln_msg + curr_index, "%02x%06x ",
				work_item->work.alpha, work_item->work.beta);
			curr_index += 9;
			int i;
			for(i=0; i<SEED_LEN; i++){
				sprintf(soln_msg + curr_index, "%02x",
					work_item->work.seed[i]);
				curr_index += 2;
			}
			soln_msg[curr_index] = ' ';
			curr_index++;
			soln_msg[curr_index] = '\0';
			sprintf(soln_msg + curr_index, "%016llx",
				(unsigned long long)curr_nonce);
			general_log_printf(LOG_EXTRA, "INFO", "found nonce = %016llx", 
				(unsigned long long)curr_nonce);

			/*
			 * avoid multiple workers responded to the client
			 */
			pthread_mutex_lock(&work_item->respond_lock);
			general_log_printf(LOG_EXTRA,
				"INFO", "get lock, ready to respond");

			/*
			 * only send back if active,
			 * if terminating,
			 * which means either client withdrew, or responded by co-worker
			 */
			if(work_item->state == active){
				general_log_printf(LOG_EXTRA, "INFO", "responding");
				respond_client(work_item->client, soln_msg);
			}

			/*
			 * turn the state the terminating,
			 * so other co-worker can know to stop
			 */
			work_item->state = terminating;

			general_log_printf(LOG_EXTRA, "INFO", "release lock");
			pthread_mutex_unlock(&work_item->respond_lock);
			break;
		}
	}
	return NULL;
}
